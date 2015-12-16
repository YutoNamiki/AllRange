// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyoPluginPrivatePCH.h"
#include "MyoDelegate.h"

DEFINE_LOG_CATEGORY(MyoPluginLog);

UMyoDelegate::UMyoDelegate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }
void IMyoDelegate::OnConnect(int32 myoId) { }
void IMyoDelegate::OnDisconnect(int32 myoId) { }
void IMyoDelegate::OnPair(int32 myoId) { }
void IMyoDelegate::OnUnpair(int32 myoId) { }
void IMyoDelegate::OnArmMoved(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose) { }
void IMyoDelegate::OnOrientationData(int32 myoId, FQuat quat) { }
void IMyoDelegate::OnOrientationData(int32 myoId, FRotator rot) { }
void IMyoDelegate::OnAccelerometerData(int32 myoId, FVector accel) { }
void IMyoDelegate::OnGyroscopeData(int32 myoId, FVector gyro) { }
void IMyoDelegate::OnPose(int32 myoId, MyoPose pose) { }
void IMyoDelegate::OnArmSync(int32 myoId, MyoArm arm, MyoArmDirection direction) { }
void IMyoDelegate::OnArmUnsync(int32 myoId) { }
void IMyoDelegate::OnEmgData(int32 myoId, FMyoEmgData data) { }
void IMyoDelegate::MyoDisabled() { }

void IMyoDelegate::MyoVibrateDevice(int32 myoId, MyoVibrationType type)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().VibrateDevice(myoId, type);
	}
}

void IMyoDelegate::MyoSetLockingPolicy(MyoLockingPolicy policy)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().SetLockingPolicy(policy);
	}
}

void IMyoDelegate::MyoSetStreamEmg(int deviceId, MyoStreamEmgType type)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().SetStreamEmg(deviceId, type);
	}
}

void IMyoDelegate::MyoUnlockMyo(int deviceId, MyoUnlockType type)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().UnlockMyo(deviceId, type);
	}
}

void IMyoDelegate::MyoLockMyo(int deviceId)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().LockMyo(deviceId);
	}
}

bool IMyoDelegate::MyoIsHubEnabled()
{
	if (IMyoPlugin::IsAvailable())
	{
		return IMyoPlugin::Get().IsHubEnabled();
	}
	return false;
}

FMyoDeviceData* IMyoDelegate::MyoLatestData(int32 myoId)
{
	if (IMyoPlugin::IsAvailable())
	{
		return IMyoPlugin::Get().LatestData(myoId);
	}
	return nullptr;
}

void IMyoDelegate::MyoWhichArm(int32 myoId, MyoArm& Arm)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().WhichArm(myoId, Arm);
	}
}

void IMyoDelegate::MyoLeftMyoId(bool& available, int32& myoId)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().LeftMyoId(available, myoId);
	}
}

void IMyoDelegate::MyoRightMyoId(bool& available, int32& myoId)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().RightMyoId(available, myoId);
	}
}

void IMyoDelegate::MyoPrimaryMyoId(bool& available, int32& myoId)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().PrimaryMyoId(available, myoId);
	}
}

int32 IMyoDelegate::MyoMaxId()
{
	auto size = 0;
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().MaxMyoId(size);
	}
	return size;
}

bool IMyoDelegate::MyoIsValidId(int32 myoId)
{
	if (IMyoPlugin::IsAvailable())
	{
		return IMyoPlugin::Get().IsValidDeviceId(myoId);
	}
	return false;
}

void IMyoDelegate::MyoConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	converted.Pitch = -1.f*orientation.Pitch;
	converted.Yaw = -1.f*orientation.Yaw;
	converted.Roll = orientation.Roll;
}

void IMyoDelegate::MyoCalibrateArmOrientation(int32 myoId, FRotator direction)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().CalibrateOrientation(myoId, direction);
	}
}

void IMyoDelegate::MyoStartup()
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().SetDelegate(this);
	}
}

void IMyoDelegate::MyoShutdown()
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().RemoveDelegate();
	}
}

void IMyoDelegate::MyoTick(float DeltaTime)
{
	if (IMyoPlugin::IsAvailable())
	{
		IMyoPlugin::Get().MyoTick(DeltaTime);
	}
}
