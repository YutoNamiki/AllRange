#pragma once

#include "GameFramework/PlayerController.h"
#include "WiiRemoteDelegateBlueprint.h"
#include "WiiRemoteInterface.h"
#include "WiiRemotePlayerController.generated.h"

UCLASS()
class AWiiRemotePlayerController : public APlayerController, public IWiiRemoteDelegateBlueprint, public IWiiRemoteInterface
{
	GENERATED_BODY()

public:
	AWiiRemotePlayerController(const FObjectInitializer& PCIP);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

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
