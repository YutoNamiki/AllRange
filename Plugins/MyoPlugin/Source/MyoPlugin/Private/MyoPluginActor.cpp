#include "MyoPluginPrivatePCH.h"
#include "Engine.h"
#include "MyoPluginActor.h"
#include "MyoComponent.h"

AMyoPluginActor::AMyoPluginActor(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	MyoComponent = CreateDefaultSubobject<UMyoComponent>(TEXT("Myo"));
}

void AMyoPluginActor::BeginPlay()
{
	Super::BeginPlay();
	auto pc = UGameplayStatics::GetPlayerController(this, 0);
	EnableInput(pc);
}

bool AMyoPluginActor::IsHubEnabled()
{
	return MyoComponent->MyoIsHubEnabled();
}

void AMyoPluginActor::SetLockingPolicy(MyoLockingPolicy policy)
{
	MyoComponent->MyoSetLockingPolicy(policy);
}

UMyoController * AMyoPluginActor::LeftMyo()
{
	return MyoComponent->MyoLeftMyo();
}

UMyoController * AMyoPluginActor::RightMyo()
{
	return MyoComponent->MyoRightMyo();
}

UMyoController * AMyoPluginActor::PrimaryMyo()
{
	return MyoComponent->MyoPrimaryMyo();
}

void AMyoPluginActor::ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	MyoComponent->MyoConvertToMyoOrientationSpace(orientation, converted);
}
