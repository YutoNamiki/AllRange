#include "MyoPluginPrivatePCH.h"
#include "MyoDelegateBlueprint.h"
#include "MyoController.h"
#include "MyoInterface.h"

UMyoDelegateBlueprint::UMyoDelegateBlueprint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { }

UMyoController* IMyoDelegateBlueprint::MyoPrimaryMyo()
{
	auto myoId = 0;
	auto available = false;
	MyoPrimaryMyoId(available, myoId);
	return InternalControllerForId(myoId);
}

UMyoController* IMyoDelegateBlueprint::MyoLeftMyo()
{
	auto myoId = 0;
	auto available = false;
	MyoLeftMyoId(available, myoId);
	return InternalControllerForId(myoId);
}

UMyoController* IMyoDelegateBlueprint::MyoRightMyo()
{
	auto myoId = 0;
	auto available = false;
	MyoRightMyoId(available, myoId);
	return InternalControllerForId(myoId);
}

void IMyoDelegateBlueprint::MyoSetLockingPolicy(MyoLockingPolicy policy)
{
	IMyoDelegate::MyoSetLockingPolicy(policy);
}

bool IMyoDelegateBlueprint::MyoIsHubEnabled()
{
	return IMyoDelegate::MyoIsHubEnabled();
}

void IMyoDelegateBlueprint::MyoConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	MyoConvertToMyoOrientationSpace(orientation, converted);
}

void IMyoDelegateBlueprint::MyoStartup()
{
	IMyoDelegate::MyoStartup();
	for (int i = 0; i < MyoMaxId(); i++)
	{
		InternalAddController(i + 1);
	}
	auto validUObject = Cast<UObject>(ValidSelfPointer);
	if (!InterfaceDelegate && validUObject)
		SetInterfaceDelegate(validUObject);
}

void IMyoDelegateBlueprint::MyoShutdown()
{
	IMyoDelegate::MyoShutdown();
}

void IMyoDelegateBlueprint::MyoTick(float DeltaTime)
{
	IMyoDelegate::MyoTick(DeltaTime);
	for (int i = 0; i < MyoMaxId(); i++)
	{
		auto controller = LatestFrame[i];
		controller->SetFromMyoDeviceData(IMyoDelegate::MyoLatestData(i + 1));
		controller->MyoId = i + 1;
	}
}

bool IMyoDelegateBlueprint::MyoIsValidId(int32 myoId)
{
	return (IsValidDelegate() && IMyoDelegate::MyoIsValidId(myoId));
}

void IMyoDelegateBlueprint::SetInterfaceDelegate(UObject* newDelegate)
{
	//UE_LOG(LogClass, Log, TEXT("InterfaceDelegate passed: %s"), *newDelegate->GetName());
	//if (newDelegate->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
	//	InterfaceDelegate = newDelegate;
	//else
	//{
	//	if (ValidSelfPointer->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
	//		InterfaceDelegate = Cast<UObject>(this);
	//	else
	//		InterfaceDelegate = nullptr;
	//	UE_LOG(LogClass, Log, TEXT("MyoDelegateBlueprint Warning: Delegate is NOT set, did your class implement HydraInterface?"));
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("MyoDelegateBlueprint Warning: Delegate is NOT set, did your class implement MyoInterface?"));
	//}
}

bool IMyoDelegateBlueprint::IsValidDelegate()
{
	return (InterfaceDelegate != nullptr);
}

void IMyoDelegateBlueprint::OnConnect(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnConnect(InterfaceDelegate, InternalControllerForId(myoId));
}

void IMyoDelegateBlueprint::OnDisconnect(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnDisconnect(InterfaceDelegate, InternalControllerForId(myoId));
}

void IMyoDelegateBlueprint::OnPair(int32 myoId)
{
	if (myoId > LatestFrame.Num())
		InternalAddController(myoId);
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnPair(InterfaceDelegate, InternalControllerForId(myoId));
}

void IMyoDelegateBlueprint::OnUnpair(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnUnpair(InterfaceDelegate, InternalControllerForId(myoId));
}

void IMyoDelegateBlueprint::OnArmMoved(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnArmMoved(InterfaceDelegate, InternalControllerForId(myoId), armAcceleration, armOrientation, armGyro, pose);
}

void IMyoDelegateBlueprint::OnOrientationData(int32 myoId, FRotator rot)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnOrientationData(InterfaceDelegate, InternalControllerForId(myoId), rot);
}

void IMyoDelegateBlueprint::OnAccelerometerData(int32 myoId, FVector accel)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnAccelerometerData(InterfaceDelegate, InternalControllerForId(myoId), accel);
}

void IMyoDelegateBlueprint::OnGyroscopeData(int32 myoId, FVector gyro)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnGyroscopeData(InterfaceDelegate, InternalControllerForId(myoId), gyro);
}

void IMyoDelegateBlueprint::OnPose(int32 myoId, MyoPose pose)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnPose(InterfaceDelegate, InternalControllerForId(myoId), pose);
}

void IMyoDelegateBlueprint::OnArmSync(int32 myoId, MyoArm arm, MyoArmDirection direction)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnArmSync(InterfaceDelegate, InternalControllerForId(myoId), arm, direction);
}

void IMyoDelegateBlueprint::OnArmUnsync(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnArmUnsync(InterfaceDelegate, InternalControllerForId(myoId));
}

void IMyoDelegateBlueprint::OnEmgData(int32 myoId, FMyoEmgData data)
{
	//if (MyoIsValidId(myoId))
	//	IMyoInterface::Execute_OnEmgData(InterfaceDelegate, InternalControllerForId(myoId), data);
}

void IMyoDelegateBlueprint::MyoDisabled()
{
	//if (IsValidDelegate())
	//	IMyoInterface::Execute_DeviceDisabled(InterfaceDelegate);
}

UMyoController* IMyoDelegateBlueprint::InternalAddController(int newId)
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

UMyoController* IMyoDelegateBlueprint::InternalControllerForId(int32 myoId)
{
	if (MyoIsValidId(myoId))
		return LatestFrame[myoId - 1];
	return nullptr;
}
