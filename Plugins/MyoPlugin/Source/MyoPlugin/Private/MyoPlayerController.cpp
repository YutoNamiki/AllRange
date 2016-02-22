#include "MyoPluginPrivatePCH.h"
#include "MyoDelegate.h"
#include "MyoPlayerController.h"
#include "MyoComponent.h"

AMyoPlayerController::AMyoPlayerController(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	MyoComponent = CreateDefaultSubobject<UMyoComponent>(TEXT("Myo"));
}

bool AMyoPlayerController::IsHubEnabled()
{
	return MyoComponent->MyoIsHubEnabled();
}

void AMyoPlayerController::SetLockingPolicy(MyoLockingPolicy policy)
{
	MyoComponent->MyoSetLockingPolicy(policy);
}

UMyoController * AMyoPlayerController::LeftMyo()
{
	return MyoComponent->MyoLeftMyo();
}

UMyoController * AMyoPlayerController::RightMyo()
{
	return MyoComponent->MyoRightMyo();
}

UMyoController * AMyoPlayerController::PrimaryMyo()
{
	return MyoComponent->MyoPrimaryMyo();
}

void AMyoPlayerController::ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted)
{
	MyoComponent->MyoConvertToMyoOrientationSpace(orientation, converted);
}
