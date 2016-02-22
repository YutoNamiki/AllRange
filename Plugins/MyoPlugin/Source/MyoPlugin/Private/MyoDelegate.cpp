// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyoPluginPrivatePCH.h"
#include "MyoDelegate.h"
#include "MyoController.h"

DEFINE_LOG_CATEGORY(MyoPluginLog);

UMyoDelegate::UMyoDelegate(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }
void IMyoDelegate::OnConnectFunction(int32 myoId) { }
void IMyoDelegate::OnDisconnectFunction(int32 myoId) { }

void IMyoDelegate::OnPairFunction(int32 myoId)
{ 
	if (myoId > LatestFrame.Num())
		InternalAddController(myoId);
}

void IMyoDelegate::OnUnpairFunction(int32 myoId) { }
void IMyoDelegate::OnArmMovedFunction(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose) { }
void IMyoDelegate::OnOrientationDataFunction(int32 myoId, FQuat quat) { }
void IMyoDelegate::OnOrientationDataFunction(int32 myoId, FRotator rot) { }
void IMyoDelegate::OnAccelerometerDataFunction(int32 myoId, FVector accel) { }
void IMyoDelegate::OnGyroscopeDataFunction(int32 myoId, FVector gyro) { }
void IMyoDelegate::OnPoseFunction(int32 myoId, MyoPose pose) { }
void IMyoDelegate::OnArmSyncFunction(int32 myoId, MyoArm arm, MyoArmDirection direction) { }
void IMyoDelegate::OnArmUnsyncFunction(int32 myoId) { }
void IMyoDelegate::OnEmgDataFunction(int32 myoId, FMyoEmgData data) { }
void IMyoDelegate::MyoDisabled() { }

UMyoController* IMyoDelegate::MyoPrimaryMyo()
{
	auto myoId = 0;
	auto available = false;
	MyoPrimaryMyoId(available, myoId);
	return InternalControllerForId(myoId);
}

UMyoController* IMyoDelegate::MyoLeftMyo()
{
	auto myoId = 0;
	auto available = false;
	MyoLeftMyoId(available, myoId);
	return InternalControllerForId(myoId);
}

UMyoController* IMyoDelegate::MyoRightMyo()
{
	auto myoId = 0;
	auto available = false;
	MyoRightMyoId(available, myoId);
	return InternalControllerForId(myoId);
}

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
	if (!IsValidDelegate())
		return false;
	if (IMyoPlugin::IsAvailable())
		return IMyoPlugin::Get().IsValidDeviceId(myoId);
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
	for (int i = 0; i < MyoMaxId(); i++)
	{
		InternalAddController(i + 1);
	}
	auto validUObject = Cast<UObject>(ValidSelfPointer);
	if (!InterfaceDelegate && validUObject)
		SetInterfaceDelegate(validUObject);
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
	for (int i = 0; i < MyoMaxId(); i++)
	{
		auto controller = LatestFrame[i];
		controller->SetFromMyoDeviceData(IMyoDelegate::MyoLatestData(i + 1));
		controller->MyoId = i + 1;
	}
}

void IMyoDelegate::SetInterfaceDelegate(UObject* newDelegate)
{
	UE_LOG(LogClass, Log, TEXT("InterfaceDelegate passed: %s"), *newDelegate->GetName());
	if (newDelegate != nullptr)
		InterfaceDelegate = newDelegate;
	else
	{
		if (ValidSelfPointer != nullptr)
			InterfaceDelegate = Cast<UObject>(this);
		else
			InterfaceDelegate = nullptr;
	}
}

bool IMyoDelegate::IsValidDelegate()
{
	return (InterfaceDelegate != nullptr);
}

UMyoController* IMyoDelegate::InternalAddController(int32 newId)
{
	auto validUObject = Cast<UObject>(ValidSelfPointer);
	UMyoController* controller;
	if (validUObject)
		controller = NewObject<UMyoController>(validUObject);
	else
		controller = NewObject<UMyoController>();
	LatestFrame.Add(controller);
	controller->myoDelegate = this;
	controller->MyoId = newId;
	return controller;
}

UMyoController* IMyoDelegate::InternalControllerForId(int32 myoId)
{
	if (MyoIsValidId(myoId))
		return LatestFrame[myoId - 1];
	return nullptr;
}
