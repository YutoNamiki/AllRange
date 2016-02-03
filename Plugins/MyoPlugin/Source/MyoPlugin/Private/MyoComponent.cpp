#include "MyoPluginPrivatePCH.h"
#include "MyoComponent.h"


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
