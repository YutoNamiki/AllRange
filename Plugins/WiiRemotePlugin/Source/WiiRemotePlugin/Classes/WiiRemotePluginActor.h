#pragma once

#include "GameFramework/Actor.h"
#include "WiiRemoteInterface.h"
#include "WiiRemotePluginActor.generated.h"

class UWiiRemoteComponent;

UCLASS()
class AWiiRemotePluginActor : public AActor, public IWiiRemoteInterface
{
	GENERATED_BODY()

public:
	UWiiRemoteComponent* WiiRemoteComponent;

	AWiiRemotePluginActor(const FObjectInitializer& PCIP);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "WiiRemote")
	void SetReportType(int32 playerIndex, ReportType reportType);
	UFUNCTION(BlueprintCallable, Category = "WiiRemote")
	void SetLED(int32 playerIndex, WiiRemoteLED ledBits);
	UFUNCTION(BlueprintCallable, Category = "WiiRemote")
	void SetVibrate(int32 playerIndex, bool on);
	UFUNCTION(BlueprintCallable, Category = "WiiRemote")
	void SetVibrateForAsync(int32 playerIndex, int32 milliseconds);
	UFUNCTION(BlueprintCallable, Category = "WiiRemote")
	void EnableSpeaker(int32 playerIndex, bool on);
	UFUNCTION(BlueprintCallable, Category = "WiiRemote")
	void PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequancy, int32 volume);
};
