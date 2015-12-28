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
	Super::OnRegister();
	ValidSelfPointer = this;
	SetInterfaceDelegate(GetOwner());
	MyoStartup();
}

void UMyoComponent::OnUnregister()
{
	Super::OnUnregister();
	MyoShutdown();
}

void UMyoComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);
	MyoTick(deltaTime);
}

bool UMyoComponent::IsHubEnabled()
{
	return MyoIsHubEnabled();
}

void UMyoComponent::SetLockingPolicy(MyoLockingPolicy policy)
{
	MyoSetLockingPolicy(policy);
}

UMyoController* UMyoComponent::LeftMyo()
{
	return MyoLeftMyo();
}

UMyoController* UMyoComponent::RightMyo()
{
	return MyoRightMyo();
}

UMyoController* UMyoComponent::PrimaryMyo()
{
	return MyoPrimaryMyo();
}

void UMyoComponent::ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	MyoConvertToMyoOrientationSpace(orientation, converted);
}

void UMyoComponent::VibrateDevice(int32 playerIndex, MyoVibrationType type)
{
	MyoVibrateDevice(playerIndex, type);
}
