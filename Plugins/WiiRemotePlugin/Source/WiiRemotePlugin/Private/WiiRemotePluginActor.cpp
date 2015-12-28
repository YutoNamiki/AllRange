#include "WiiRemotePluginPrivatePCH.h"
#include "Engine.h"
#include "WiiRemotePluginActor.h"

AWiiRemotePluginActor::AWiiRemotePluginActor(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWiiRemotePluginActor::BeginPlay()
{
	Super::BeginPlay();
	ValidSelfPointer = this;
	Startup();
}

void AWiiRemotePluginActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Shutdown();
}

void AWiiRemotePluginActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IWiiRemoteDelegateBlueprint::Tick(DeltaTime);
}

void AWiiRemotePluginActor::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	IWiiRemoteDelegate::SetLED(playerIndex, ledBits);
}

void AWiiRemotePluginActor::SetVibrate(int32 playerIndex, bool on)
{
	IWiiRemoteDelegate::SetRumble(playerIndex, on);
}

void AWiiRemotePluginActor::SetVibrateForAsync(int32 playerIndex, int32 milliseconds)
{
	IWiiRemoteDelegate::SetRumbleForAsync(playerIndex, milliseconds);
}

void AWiiRemotePluginActor::EnableSpeaker(int32 playerIndex, bool on)
{
	IWiiRemoteDelegate::EnableSpeaker(playerIndex, on);
}

void AWiiRemotePluginActor::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequancy, int32 volume)
{
	IWiiRemoteDelegate::PlaySquareWave(playerIndex, frequancy, volume);
}

