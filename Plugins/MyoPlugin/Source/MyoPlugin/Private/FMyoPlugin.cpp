// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyoPluginPrivatePCH.h"
#include "FMyoPlugin.h"
#include "MyoDelegate.h"
#include "DataCollector.h"

static const FString PluginVersion = "ver.SgnmkOriginal";

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
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseRest, LOCTEXT("MyoPoseRest", "Myo Pose Rest"), FKeyDetails::GamepadKey, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseFist, LOCTEXT("MyoPoseFist", "Myo Pose Fist"), FKeyDetails::GamepadKey, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseWaveIn, LOCTEXT("MyoPoseWaveIn", "Myo Pose Wave In"), FKeyDetails::GamepadKey, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseWaveOut, LOCTEXT("MyoPoseWaveOut", "Myo Pose Wave Out"), FKeyDetails::GamepadKey, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseFingersSpread, LOCTEXT("MyoPoseFingersSpread", "Myo Pose FingersSpread"), FKeyDetails::GamepadKey, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseDoubleTap, LOCTEXT("MyoPoseDoubleTap", "Myo Pose Double Tap"), FKeyDetails::GamepadKey, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::PoseUnknown, LOCTEXT("MyoPoseUnknown", "Myo Pose Unknown"), FKeyDetails::GamepadKey, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::AccelerationX, LOCTEXT("MyoAccelerationX", "Myo Acceleration X"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::AccelerationY, LOCTEXT("MyoAccelerationY", "Myo Acceleration Y"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::AccelerationZ, LOCTEXT("MyoAccelerationZ", "Myo Acceleration Z"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::OrientationPitch, LOCTEXT("MyoOrientationPitch", "Myo Orientation Pitch"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::OrientationYaw, LOCTEXT("MyoOrientationYaw", "Myo Orientation Yaw"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::OrientationRoll, LOCTEXT("MyoOrientationRoll", "Myo Orientation Roll"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::GyroX, LOCTEXT("MyoGyroX", "Myo Gyro X"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::GyroY, LOCTEXT("MyoGyroY", "Myo Gyro Y"), FKeyDetails::FloatAxis, FName("MyoArmband")));
	EKeys::AddKey(FKeyDetails(EMyoKeys::GyroZ, LOCTEXT("MyoGyroZ", "Myo Gyro Z"), FKeyDetails::FloatAxis, FName("MyoArmband")));
}

void FMyoPlugin::ShutdownModule()
{

}

void FMyoPlugin::SetDelegate(IMyoDelegate* newDelegate)
{
	collector->MyoDelegate = newDelegate;
	collector->Startup();
	if(!collector->Enabled) 
	{
		collector->MyoDelegate->MyoDisabled();
		UE_LOG(MyoPluginLog, Warning, TEXT("Myo is Disabled."));
	}
	//else
	//	collector->SetLockingPolicy(myo::Hub::lockingPolicyNone);
	UE_LOG(MyoPluginLog, Log, TEXT("Myo Delegate Set (should only be called once per begin play or you have duplicates)."));
}

void FMyoPlugin::RemoveDelegate()
{
	collector->MyoDelegate = nullptr;
	collector->ShutDown();
}

void FMyoPlugin::MyoTick(float DeltaTime)
{
	collector->Tick(DeltaTime);
}

void FMyoPlugin::VibrateDevice(int32 deviceId, MyoVibrationType vibrationType)
{
	if (!this->IsValidDeviceId(deviceId))
		return;
	auto myoId = collector->KnownMyos[deviceId - 1];
	collector->VibrateDevice(myoId, vibrationType);
}

FMyoDeviceData* FMyoPlugin::LatestData(int32 deviceId)
{
	if (!this->IsValidDeviceId(deviceId)) 
		return nullptr;
	return &(collector->Data[deviceId - 1]);
}

void FMyoPlugin::WhichArm(int32 deviceId, MyoArm& arm)
{
	if (!this->IsValidDeviceId(deviceId)) 
		return;
	arm = collector->Data[deviceId - 1].Arm;
}

void FMyoPlugin::LeftMyoId(bool& available, int32& deviceId)
{
	if (collector->LeftMyo == -1)
		available = false;
	else 
	{
		available = true;
		deviceId = collector->LeftMyo;
	}
}

void FMyoPlugin::RightMyoId(bool& available, int32& deviceId)
{
	if (collector->RightMyo == -1)
		available = false;
	else 
	{
		available = true;
		deviceId = collector->RightMyo;
	}
}

void FMyoPlugin::PrimaryMyoId(bool& available, int32& deviceId)
{
	deviceId = collector->IdentifyMyo(collector->LastValidMyo());
	available = (deviceId != -1);
}

void FMyoPlugin::MaxMyoId(int32& maxId)
{
	maxId = collector->Data.Num();
}

bool FMyoPlugin::IsHubEnabled()
{
	return collector->Enabled;
}

bool FMyoPlugin::IsValidDeviceId(int32 deviceId)
{
	return !(deviceId < 1 || deviceId > collector->Data.Num());
}

void FMyoPlugin::CalibrateOrientation(int32 deviceId, FRotator direction)
{
	if (deviceId == 0)
	{
		for (auto i = 0; i < collector->Data.Num(); ++i)
		{
			collector->Data[i].ArmSpaceCorrection = CombineRotators(collector->Data[i].Orientation * -1.0f, direction);
			collector->ArmSpaceCorrection = collector->Data[i].ArmSpaceCorrection;
			collector->CorrectionAvailable = true;
		}
	}
	else
	{
		if (!this->IsValidDeviceId(deviceId)) 
			return;
		collector->Data[deviceId - 1].ArmSpaceCorrection = CombineRotators(collector->Data[deviceId - 1].Orientation * -1.0f, direction);
		collector->ArmSpaceCorrection = collector->Data[deviceId - 1].ArmSpaceCorrection;
		collector->CorrectionAvailable = true;
	}
}

void FMyoPlugin::SetLockingPolicy(MyoLockingPolicy policy)
{
	collector->ResetHub();
	UE_LOG(MyoPluginLog, Log, TEXT("Set Policy to %d"), static_cast<int32>(policy));
	collector->SetLockingPolicy(policy);
}

void FMyoPlugin::UnlockMyo(int32 deviceId, MyoUnlockType type)
{
	if (!this->IsValidDeviceId(deviceId))
		return;
	auto myoId = collector->KnownMyos[deviceId - 1];
	collector->UnlockMyo(myoId, type);
}

void FMyoPlugin::LockMyo(int32 deviceId)
{
	if (!this->IsValidDeviceId(deviceId))
		return;
	auto myoId = collector->KnownMyos[deviceId - 1];
	collector->LockMyo(myoId);
}

void FMyoPlugin::SetStreamEmg(int32 deviceId, MyoStreamEmgType type)
{
	if (!this->IsValidDeviceId(deviceId))
		return;
	auto myoId = collector->KnownMyos[deviceId - 1];
	collector->SetStreamEmg(myoId, type);
}

FRotator FMyoPlugin::CombineRotators(FRotator a, FRotator b)
{
	return FRotator(FQuat(b) * FQuat(a));
}

#undef LOCTEXT_NAMESPACE
