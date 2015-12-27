#include "MyoPluginPrivatePCH.h"
#include "Engine.h"
#include "MyoPluginActor.h"

AMyoPluginActor::AMyoPluginActor(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyoPluginActor::BeginPlay()
{
	Super::BeginPlay();
	auto pc = UGameplayStatics::GetPlayerController(this, 0);
	EnableInput(pc);
	ValidSelfPointer = this;
	MyoStartup();
}

void AMyoPluginActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	MyoShutdown();
}

void AMyoPluginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyoTick(DeltaTime);
}

bool AMyoPluginActor::IsHubEnabled()
{
	return MyoIsHubEnabled();
}

void AMyoPluginActor::SetLockingPolicy(MyoLockingPolicy policy)
{
	MyoSetLockingPolicy(policy);
}

UMyoController * AMyoPluginActor::LeftMyo()
{
	return MyoLeftMyo();
}

UMyoController * AMyoPluginActor::RightMyo()
{
	return MyoRightMyo();
}

UMyoController * AMyoPluginActor::PrimaryMyo()
{
	return MyoPrimaryMyo();
}

void AMyoPluginActor::ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	MyoConvertToMyoOrientationSpace(orientation, converted);
}
