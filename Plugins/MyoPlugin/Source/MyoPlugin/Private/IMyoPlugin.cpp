// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyoPluginPrivatePCH.h"
#include "MyoPluginObject.h"
#include "MyoDelegate.h"
#include "DataCollector.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

/* êÈåæïî */

static const FString PluginVersion = "ver.SgnmkOriginal";
static const FRotator OrientationScale = FRotator(1.0f / 90.0f, 1.0f / 180.0f, 1.0f / 180.0f);
static const float GyroScale = 1.0f / 45.0f;

struct EMyoKeys
{
	static const FKey PoseRest;
	static const FKey PoseFist;
	static const FKey PoseWaveIn;
	static const FKey PoseWaveOut;
	static const FKey PoseFingersSpread;
	static const FKey PoseDoubleTap;
	static const FKey PoseUnknown;
	static const FKey AccelerationX;
	static const FKey AccelerationY;
	static const FKey AccelerationZ;
	static const FKey OrientationPitch;
	static const FKey OrientationYaw;
	static const FKey OrientationRoll;
	static const FKey GyroX;
	static const FKey GyroY;
	static const FKey GyroZ;
};

const FKey EMyoKeys::PoseRest("MyoPoseRest");
const FKey EMyoKeys::PoseFist("MyoPoseFist");
const FKey EMyoKeys::PoseWaveIn("MyoPoseWaveIn");
const FKey EMyoKeys::PoseWaveOut("MyoPoseWaveOut");
const FKey EMyoKeys::PoseFingersSpread("MyoPoseFingersSpread");
const FKey EMyoKeys::PoseDoubleTap("MyoPoseDoubleTap");
const FKey EMyoKeys::PoseUnknown("MyoPoseUnknown");
const FKey EMyoKeys::AccelerationX("MyoAccelerationX");
const FKey EMyoKeys::AccelerationY("MyoAccelerationY");
const FKey EMyoKeys::AccelerationZ("MyoAccelerationZ");
const FKey EMyoKeys::OrientationPitch("MyoOrientationPitch");
const FKey EMyoKeys::OrientationYaw("MyoOrientationYaw");
const FKey EMyoKeys::OrientationRoll("MyoOrientationRoll");
const FKey EMyoKeys::GyroX("MyoGyroX");
const FKey EMyoKeys::GyroY("MyoGyroY");
const FKey EMyoKeys::GyroZ("MyoGyroZ");

bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat);
bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat);
bool EmitAnalogInputEventForKey(FKey key, float value, int user);
FRotator ConvertOrientationToUE(FRotator rawOrientation);
FRotator CombineRotators(FRotator a, FRotator b);
FVector ConvertVectorToUE(FVector rawAcceleration);
FVector ConvertAccelerationToBodySpace(FVector armAcceleration, FRotator orientation, FRotator armCorrection, MyoArmDirection direction);
FRotator ConvertOrientationToArmSpace(FRotator convertedOrientation, FRotator armCorrection, MyoArmDirection direction);

class FMyoPlugin : public IMyoPlugin
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void SetDelegate(IMyoDelegate* newDelegate) override;
	virtual void RemoveDelegate() override;
	virtual void MyoTick(float DeltaTime) override;
	virtual void VibrateDevice(int32 deviceId, MyoVibrationType vibrationType) override;
	virtual FMyoDeviceData* LatestData(int32 deviceId) override;
	virtual void WhichArm(int32 deviceId, MyoArm& arm) override;
	virtual void LeftMyoId(bool& available, int32& deviceId) override;
	virtual void RightMyoId(bool& available, int32& deviceId) override;
	virtual void PrimaryMyoId(bool& available, int32& deviceId) override;
	virtual void MaxMyoId(int32& maxId) override;
	virtual bool IsHubEnabled() override;
	virtual bool IsValidDeviceId(int32 deviceId) override;
	virtual void CalibrateOrientation(int32 deviceId, FRotator direction) override;
	virtual void SetLockingPolicy(MyoLockingPolicy policy) override;
	virtual void UnlockMyo(int32 deviceId, MyoUnlockType type) override;
	virtual void LockMyo(int32 deviceId) override;
	virtual void SetStreamEmg(int32 deviceId, MyoStreamEmgType type) override;

private:
	UDataCollector* collector;
};

/* íËã`ïî */

IMPLEMENT_MODULE(FMyoPlugin, MyoPlugin )

#define LOCTEXT_NAMESPACE "MyoPlugin"

bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int user)
{
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, false, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}

FRotator ConvertOrientationToUE(FRotator rawOrientation)
{
	return FRotator(rawOrientation.Pitch * -1.0f, rawOrientation.Yaw * -1.0f, rawOrientation.Roll);
}

FRotator CombineRotators(FRotator a, FRotator b)
{
	return FRotator(FQuat(b) * FQuat(a));
}

FVector ConvertVectorToUE(FVector rawAcceleration)
{
	return FVector(rawAcceleration.X, -rawAcceleration.Y, rawAcceleration.Z);
}

FVector ConvertAccelerationToBodySpace(FVector armAcceleration, FRotator orientation, FRotator armCorrection, MyoArmDirection direction)
{
	auto directionModifier = 1.0f;
	if (direction == MyoArmDirection::Elbow)
		directionModifier = -1.0f;
	auto armYawCorrection = FRotator(0.0f, armCorrection.Yaw, 0.0f);
	auto fullCompensationOrientation = CombineRotators(orientation, armYawCorrection);
	auto reactionAcceleration = fullCompensationOrientation.RotateVector(armAcceleration);
	return ((reactionAcceleration * FVector(directionModifier, directionModifier, 1.0f)) + FVector(0.0f, 0.0f, 1.0f)) * -1.0f;
}

FRotator ConvertOrientationToArmSpace(FRotator convertedOrientation, FRotator armCorrection, MyoArmDirection direction)
{
	auto directionModifier = 1.0f;
	if (direction == MyoArmDirection::Elbow)
	{
		directionModifier = -1.0f;
		convertedOrientation = FRotator(convertedOrientation.Pitch * directionModifier, convertedOrientation.Yaw, convertedOrientation.Roll * directionModifier);
	}
	auto tempRot = CombineRotators(FRotator(0.0f, 0.0f, armCorrection.Roll*directionModifier), convertedOrientation);
	return CombineRotators(tempRot, FRotator(0.0f, armCorrection.Yaw, 0.0f));
}

void FMyoPlugin::StartupModule()
{
	UE_LOG(MyoPluginLog, Log, TEXT("Using Myo Plugin version %s"), *PluginVersion);
	collector = NewObject<UDataCollector>();
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseRest, LOCTEXT("MyoPoseRest", "Myo Pose Rest"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseFist, LOCTEXT("MyoPoseFist", "Myo Pose Fist"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseWaveIn, LOCTEXT("MyoPoseWaveIn", "Myo Pose Wave In"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseWaveOut, LOCTEXT("MyoPoseWaveOut", "Myo Pose Wave Out"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseFingersSpread, LOCTEXT("MyoPoseFingersSpread", "Myo Pose FingersSpread"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseDoubleTap, LOCTEXT("MyoPoseDoubleTap", "Myo Pose Double Tap"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseUnknown, LOCTEXT("MyoPoseUnknown", "Myo Pose Unknown"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(EMyoKeys::AccelerationX, LOCTEXT("MyoAccelerationX", "Myo Acceleration X"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::AccelerationY, LOCTEXT("MyoAccelerationY", "Myo Acceleration Y"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::AccelerationZ, LOCTEXT("MyoAccelerationZ", "Myo Acceleration Z"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::OrientationPitch, LOCTEXT("MyoOrientationPitch", "Myo Orientation Pitch"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::OrientationYaw, LOCTEXT("MyoOrientationYaw", "Myo Orientation Yaw"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::OrientationRoll, LOCTEXT("MyoOrientationRoll", "Myo Orientation Roll"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::GyroX, LOCTEXT("MyoGyroX", "Myo Gyro X"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::GyroY, LOCTEXT("MyoGyroY", "Myo Gyro Y"), FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(EMyoKeys::GyroZ, LOCTEXT("MyoGyroZ", "Myo Gyro Z"), FKeyDetails::FloatAxis));
}

void FMyoPlugin::ShutdownModule()
{
	collector->MyoDelegate = nullptr;
	collector->ShutDown();
}

void FMyoPlugin::SetDelegate(IMyoDelegate* newDelegate)
{
	collector->MyoDelegate = newDelegate;
	collector->Startup();
}

void FMyoPlugin::RemoveDelegate()
{

}

void FMyoPlugin::MyoTick(float DeltaTime)
{
	collector->Tick(DeltaTime);

}

void FMyoPlugin::VibrateDevice(int32 deviceId, MyoVibrationType vibrationType)
{

}

FMyoDeviceData* FMyoPlugin::LatestData(int32 deviceId)
{

	return nullptr;
}

void FMyoPlugin::WhichArm(int32 deviceId, MyoArm& arm)
{

}

void FMyoPlugin::LeftMyoId(bool& available, int32& deviceId)
{

}

void FMyoPlugin::RightMyoId(bool& available, int32& deviceId)
{

}

void FMyoPlugin::PrimaryMyoId(bool& available, int32& deviceId)
{

}

void FMyoPlugin::MaxMyoId(int32& maxId)
{

}

bool FMyoPlugin::IsHubEnabled()
{

	return false;
}

bool FMyoPlugin::IsValidDeviceId(int32 deviceId)
{

	return false;
}

void FMyoPlugin::CalibrateOrientation(int32 deviceId, FRotator direction)
{

}

void FMyoPlugin::SetLockingPolicy(MyoLockingPolicy policy)
{

}

void FMyoPlugin::UnlockMyo(int32 deviceId, MyoUnlockType type)
{

}

void FMyoPlugin::LockMyo(int32 deviceId)
{

}

void FMyoPlugin::SetStreamEmg(int32 deviceId, MyoStreamEmgType type)
{

}

#undef LOCTEXT_NAMESPACE
