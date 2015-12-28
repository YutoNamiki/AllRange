// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "WiiRemoteDelegateBlueprint.h"
#include "WiiRemoteComponent.generated.h"
 
UCLASS(Blueprintable, ClassGroup = "Input Controller", meta = (BlueprintSpawnableComponent))
class WIIREMOTEPLUGIN_API UWiiRemoteComponent : public UActorComponent, public IWiiRemoteDelegateBlueprint
{
	GENERATED_BODY()

public:
	UWiiRemoteComponent(const FObjectInitializer& init);
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
	
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
