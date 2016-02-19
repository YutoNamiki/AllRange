#include "MyoPluginPrivatePCH.h"
#include "MyoComponent.h"
#include "MyoController.h"

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

void UMyoComponent::OnConnect(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	MyoOnConnect.Broadcast(Cast<UMyoController>(InterfaceDelegate));
}
void UMyoComponent::OnDisconnect(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnDisconnect(InterfaceDelegate, InternalControllerForId(myoId));
}

void UMyoComponent::OnPair(int32 myoId)
{
	if (myoId > LatestFrame.Num())
		InternalAddController(myoId);
	//if (MyoIsValidId(myoId))
	//	Execute_OnPair(InterfaceDelegate, InternalControllerForId(myoId));
}

void UMyoComponent::OnUnpair(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnUnpair(InterfaceDelegate, InternalControllerForId(myoId));
}

void UMyoComponent::OnArmMoved(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnArmMoved(InterfaceDelegate, InternalControllerForId(myoId), armAcceleration, armOrientation, armGyro, pose);
}

void UMyoComponent::OnOrientationData(int32 myoId, FRotator rot)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnOrientationData(InterfaceDelegate, InternalControllerForId(myoId), rot);
}

void UMyoComponent::OnAccelerometerData(int32 myoId, FVector accel)
{
	if (MyoIsValidId(myoId))
		MyoOnAccelerometerData.Broadcast(Cast<UMyoController>(InterfaceDelegate), accel);
}

void UMyoComponent::OnGyroscopeData(int32 myoId, FVector gyro)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnGyroscopeData(InterfaceDelegate, InternalControllerForId(myoId), gyro);
}

void UMyoComponent::OnPose(int32 myoId, MyoPose pose)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnPose(InterfaceDelegate, InternalControllerForId(myoId), pose);
}

void UMyoComponent::OnArmSync(int32 myoId, MyoArm arm, MyoArmDirection direction)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnArmSync(InterfaceDelegate, InternalControllerForId(myoId), arm, direction);
}

void UMyoComponent::OnArmUnsync(int32 myoId)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnArmUnsync(InterfaceDelegate, InternalControllerForId(myoId));
}

void UMyoComponent::OnEmgData(int32 myoId, FMyoEmgData data)
{
	//if (MyoIsValidId(myoId))
	//	Execute_OnEmgData(InterfaceDelegate, InternalControllerForId(myoId), data);
}

void UMyoComponent::MyoDisabled()
{
	//if (IsValidDelegate())
	//	Execute_DeviceDisabled(InterfaceDelegate);
}