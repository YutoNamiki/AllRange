#include "MyoPluginPrivatePCH.h"
#include "MyoComponent.h"
#include "MyoInterface.h"

UMyoComponent::UMyoComponent(const FObjectInitializer& init) : Super(init)
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyoComponent::OnRegister()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::OnRegister"));
	Super::OnRegister();
	ValidSelfPointer = this;
	SetInterfaceDelegate(GetOwner());
	MyoStartup();
}

void UMyoComponent::OnUnregister()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::OnUnregister"));
	Super::OnUnregister();
	MyoShutdown();
}

void UMyoComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::TickComponent"));
	Super::TickComponent(deltaTime, tickType, thisTickFunction);
	MyoTick(deltaTime);
}

bool UMyoComponent::IsHubEnabled()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::IsHubEnabled"));
	return MyoIsHubEnabled();
}

void UMyoComponent::SetLockingPolicy(MyoLockingPolicy policy)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::SetLockingPolicy"));
	MyoSetLockingPolicy(policy);
}

UMyoController* UMyoComponent::LeftMyo()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::LeftMyo"));
	return MyoLeftMyo();
}

UMyoController* UMyoComponent::RightMyo()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::RightMyo"));
	return MyoRightMyo();
}

UMyoController* UMyoComponent::PrimaryMyo()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::PrimaryMyo"));
	return MyoPrimaryMyo();
}

void UMyoComponent::ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::ConvertToMyoOrientationSpace"));
	MyoConvertToMyoOrientationSpace(orientation, converted);
}

void UMyoComponent::VibrateDevice(int32 playerIndex, MyoVibrationType type)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UMyoComponent::VibrateDevice"));
	MyoVibrateDevice(playerIndex, type);
}

void UMyoComponent::OnConnectFunction(int32 myoId)
{
	if (MyoIsValidId(myoId))
	{
		OnConnect.Broadcast(InternalControllerForId(myoId));
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnConnect(InterfaceDelegate, InternalControllerForId(myoId));
	}
}

void UMyoComponent::OnDisconnectFunction(int32 myoId)
{
	if (MyoIsValidId(myoId))
	{
		OnDisconnect.Broadcast(InternalControllerForId(myoId));
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnDisconnect(InterfaceDelegate, InternalControllerForId(myoId));
	}
}

void UMyoComponent::OnPairFunction(int32 myoId)
{
	IMyoDelegate::OnPairFunction(myoId);
	if (MyoIsValidId(myoId))
	{
		OnPair.Broadcast(InternalControllerForId(myoId));
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnPair(InterfaceDelegate, InternalControllerForId(myoId));
	}
}

void UMyoComponent::OnUnpairFunction(int32 myoId)
{
	if (MyoIsValidId(myoId))
	{
		OnUnpair.Broadcast(InternalControllerForId(myoId));
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnUnpair(InterfaceDelegate, InternalControllerForId(myoId));
	}
}

void UMyoComponent::OnArmMovedFunction(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose)
{
	if (MyoIsValidId(myoId))
	{
		OnArmMoved.Broadcast(InternalControllerForId(myoId), armAcceleration, armOrientation, armGyro, pose);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnArmMoved(InterfaceDelegate, InternalControllerForId(myoId), armAcceleration, armOrientation, armGyro, pose);
	}
}

void UMyoComponent::OnOrientationDataFunction(int32 myoId, FQuat quat)
{
	if (MyoIsValidId(myoId))
	{
		OnQuaternionData.Broadcast(InternalControllerForId(myoId), quat);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnOrientationData(InterfaceDelegate, InternalControllerForId(myoId), FRotator(quat));
	}
}

void UMyoComponent::OnOrientationDataFunction(int32 myoId, FRotator rot)
{
	if (MyoIsValidId(myoId))
	{
		OnOrientationData.Broadcast(InternalControllerForId(myoId), rot);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnOrientationData(InterfaceDelegate, InternalControllerForId(myoId), rot);
	}
}

void UMyoComponent::OnAccelerometerDataFunction(int32 myoId, FVector accel)
{
	if (MyoIsValidId(myoId))
	{
		OnAccelerometerData.Broadcast(InternalControllerForId(myoId), accel);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnAccelerometerData(InterfaceDelegate, InternalControllerForId(myoId), accel);
	}
}

void UMyoComponent::OnGyroscopeDataFunction(int32 myoId, FVector gyro)
{
	if (MyoIsValidId(myoId))
	{
		OnGyroscopeData.Broadcast(InternalControllerForId(myoId), gyro);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnGyroscopeData(InterfaceDelegate, InternalControllerForId(myoId), gyro);
	}
}

void UMyoComponent::OnPoseFunction(int32 myoId, MyoPose pose)
{
	if (MyoIsValidId(myoId))
	{
		OnPose.Broadcast(InternalControllerForId(myoId), pose);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnPose(InterfaceDelegate, InternalControllerForId(myoId), pose);
	}
}

void UMyoComponent::OnArmSyncFunction(int32 myoId, MyoArm arm, MyoArmDirection direction)
{
	if (MyoIsValidId(myoId))
	{
		OnArmSync.Broadcast(InternalControllerForId(myoId), arm, direction);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnArmSync(InterfaceDelegate, InternalControllerForId(myoId), arm, direction);
	}
}

void UMyoComponent::OnArmUnsyncFunction(int32 myoId)
{
	if (MyoIsValidId(myoId))
	{
		OnArmUnsync.Broadcast(InternalControllerForId(myoId));
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnArmUnsync(InterfaceDelegate, InternalControllerForId(myoId));
	}
}

void UMyoComponent::OnEmgDataFunction(int32 myoId, FMyoEmgData data)
{
	if (MyoIsValidId(myoId))
	{
		OnEmgData.Broadcast(InternalControllerForId(myoId), data);
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_OnEmgData(InterfaceDelegate, InternalControllerForId(myoId), data);
	}
}

void UMyoComponent::MyoDisabled()
{
	if (IsValidDelegate())
	{
		OnDisabled.Broadcast();
		if (GetOwner()->GetClass()->ImplementsInterface(UMyoInterface::StaticClass()))
			IMyoInterface::Execute_DeviceDisabled(InterfaceDelegate);
	}
}
