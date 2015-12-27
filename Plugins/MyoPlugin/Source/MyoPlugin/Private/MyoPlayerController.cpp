#include "MyoPluginPrivatePCH.h"
#include "MyoDelegate.h"
#include "MyoPlayerController.h"

AMyoPlayerController::AMyoPlayerController(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyoPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ValidSelfPointer = this;
	MyoStartup();
}

void AMyoPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	MyoShutdown();
}

void AMyoPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyoTick(DeltaTime);
}

bool AMyoPlayerController::IsHubEnabled()
{
	return MyoIsHubEnabled();
}

void AMyoPlayerController::SetLockingPolicy(MyoLockingPolicy policy)
{
	MyoSetLockingPolicy(policy);
}

UMyoController * AMyoPlayerController::LeftMyo()
{
	return MyoLeftMyo();
}

UMyoController * AMyoPlayerController::RightMyo()
{
	return MyoRightMyo();
}

UMyoController * AMyoPlayerController::PrimaryMyo()
{
	return MyoPrimaryMyo();
}

void AMyoPlayerController::ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	MyoConvertToMyoOrientationSpace(orientation, converted);
}
