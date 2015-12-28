#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteDelegate.h"
#include "WiiRemotePlayerController.h"

AWiiRemotePlayerController::AWiiRemotePlayerController(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWiiRemotePlayerController::BeginPlay()
{
	Super::BeginPlay();
	ValidSelfPointer = this;
	Startup();
}

void AWiiRemotePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Shutdown();
}

void AWiiRemotePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IWiiRemoteDelegateBlueprint::Tick(DeltaTime);
}

void AWiiRemotePlayerController::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	IWiiRemoteDelegate::SetLED(playerIndex, ledBits);
}

void AWiiRemotePlayerController::SetVibrate(int32 playerIndex, bool on)
{
	IWiiRemoteDelegate::SetRumble(playerIndex, on);
}

void AWiiRemotePlayerController::SetVibrateForAsync(int32 playerIndex, int32 milliseconds)
{
	IWiiRemoteDelegate::SetRumbleForAsync(playerIndex, milliseconds);
}

void AWiiRemotePlayerController::EnableSpeaker(int32 playerIndex, bool on)
{
	IWiiRemoteDelegate::EnableSpeaker(playerIndex, on);
}

void AWiiRemotePlayerController::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequancy, int32 volume)
{
	IWiiRemoteDelegate::PlaySquareWave(playerIndex, frequancy, volume);
}
