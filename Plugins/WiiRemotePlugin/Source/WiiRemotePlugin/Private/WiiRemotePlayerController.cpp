#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteDelegate.h"
#include "WiiRemotePlayerController.h"
#include "WiiRemoteComponent.h"

AWiiRemotePlayerController::AWiiRemotePlayerController(const FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	WiiRemoteComponent = CreateDefaultSubobject<UWiiRemoteComponent>(TEXT("WiiRemote"));
}

void AWiiRemotePlayerController::BeginPlay()
{
	Super::BeginPlay();
	WiiRemoteComponent->ValidSelfPointer = this;
	WiiRemoteComponent->Startup();
}

void AWiiRemotePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	WiiRemoteComponent->Shutdown();
}

void AWiiRemotePlayerController::SetReportType(int32 playerIndex, ReportType reportType)
{
	WiiRemoteComponent->SetReportType(playerIndex, reportType);
}

void AWiiRemotePlayerController::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	WiiRemoteComponent->SetLED(playerIndex, ledBits);
}

void AWiiRemotePlayerController::SetVibrate(int32 playerIndex, bool on)
{
	WiiRemoteComponent->SetRumble(playerIndex, on);
}

void AWiiRemotePlayerController::SetVibrateForAsync(int32 playerIndex, int32 milliseconds)
{
	WiiRemoteComponent->SetRumbleForAsync(playerIndex, milliseconds);
}

void AWiiRemotePlayerController::EnableSpeaker(int32 playerIndex, bool on)
{
	WiiRemoteComponent->EnableSpeaker(playerIndex, on);
}

void AWiiRemotePlayerController::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequancy, int32 volume)
{
	WiiRemoteComponent->PlaySquareWave(playerIndex, frequancy, volume);
}
