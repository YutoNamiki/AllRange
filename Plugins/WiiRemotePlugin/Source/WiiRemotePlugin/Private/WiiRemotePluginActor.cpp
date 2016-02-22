#include "WiiRemotePluginPrivatePCH.h"
#include "Engine.h"
#include "WiiRemotePluginActor.h"
#include "WiiRemoteComponent.h"

AWiiRemotePluginActor::AWiiRemotePluginActor(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	WiiRemoteComponent = CreateDefaultSubobject<UWiiRemoteComponent>(TEXT("WiiRemote"));
}

void AWiiRemotePluginActor::BeginPlay()
{
	Super::BeginPlay();
	WiiRemoteComponent->ValidSelfPointer = this;
	WiiRemoteComponent->Startup();
}

void AWiiRemotePluginActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	WiiRemoteComponent->Shutdown();
}

void AWiiRemotePluginActor::SetReportType(int32 playerIndex, ReportType reportType)
{
	WiiRemoteComponent->SetReportType(playerIndex, reportType);
}

void AWiiRemotePluginActor::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	WiiRemoteComponent->SetLED(playerIndex, ledBits);
}

void AWiiRemotePluginActor::SetVibrate(int32 playerIndex, bool on)
{
	WiiRemoteComponent->SetRumble(playerIndex, on);
}

void AWiiRemotePluginActor::SetVibrateForAsync(int32 playerIndex, int32 milliseconds)
{
	WiiRemoteComponent->SetRumbleForAsync(playerIndex, milliseconds);
}

void AWiiRemotePluginActor::EnableSpeaker(int32 playerIndex, bool on)
{
	WiiRemoteComponent->EnableSpeaker(playerIndex, on);
}

void AWiiRemotePluginActor::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequancy, int32 volume)
{
	WiiRemoteComponent->PlaySquareWave(playerIndex, frequancy, volume);
}
