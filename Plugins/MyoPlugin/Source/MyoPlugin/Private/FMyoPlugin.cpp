// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyoPluginPrivatePCH.h"
#include "FMyoPlugin.h"
#include "MyoDelegate.h"
#include "DataCollector.h"

static const FString PluginVersion = "ver.SgnmkOriginal";
static const FRotator OrientationScale = FRotator(1.0f / 90.0f, 1.0f / 180.0f, 1.0f / 180.0f);
static const float GyroScale = 1.0f / 45.0f;

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

IMPLEMENT_MODULE(FMyoPlugin, MyoPlugin )

#define LOCTEXT_NAMESPACE "MyoPlugin"

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
