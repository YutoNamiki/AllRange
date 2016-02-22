// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "WiiRemoteDelegate.h"
#include "WiiRemoteComponent.generated.h"
 
UCLASS(Blueprintable, ClassGroup = "Input Controller", meta = (BlueprintSpawnableComponent))
class WIIREMOTEPLUGIN_API UWiiRemoteComponent : public UActorComponent, public IWiiRemoteDelegate
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FConnectedSignature OnConnected;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FConnectionLostSignature OnConnectionLost;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FBatteryChangedSignature OnBatteryChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FBatteryDrainedSignature OnBatteryDrained;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FLEDsChangedSignature OnLEDsChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FButtonsChangedSignature OnButtonsChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FAccelChangedSignature OnAccelChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FOrientationChangedSignature OnOrientationChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FIRChangedSignature OnIRChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FNunchukConnectedSignature OnNunchukConnected;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FNunchukButtonsChangedSignature OnNunchukButtonsChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FNunchukAccelChangedSignature OnNunchukAccelChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FNunchukOrientationChangedSignature OnNunchukOrientationChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FNunchukJoystickChangedSignature OnNunchukJoystickChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FClassicConnectedSignature OnClassicConnected;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FClassicButtonsChangedSignature OnClassicButtonsChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FClassicJoystickLChangedSignature OnClassicJoystickLChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FClassicJoystickRChangedSignature OnClassicJoystickRChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FClassicTriggersChangedSignature OnClassicTriggersChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FBalanceConnectedSignature OnBalanceConnected;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FBalanceWeightChangedSignature OnBalanceWeightChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FMotionPlusDetectedSignature OnMotionPlusDetected;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FMotionPlusEnabledSignature OnMotionPlusEnabled;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FMotionPlusSpeedChangedSignature OnMotionPlusSpeedChanged;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FMotionPlusExtensionConnectedSignature OnMotionPlusExtensionConnected;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FMotionPlusExtensionDisconnectedSignature OnMotionPlusExtensionDisconnected;
	UPROPERTY(BlueprintAssignable, Category = "WiiRemote")
	FExtensionDisconnectedSignature OnExtensionDisconnected;

	UWiiRemoteComponent(const FObjectInitializer& init);
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
	
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

	virtual void OnConnectedFunction(int32 wiiRemoteId) override;
	virtual void OnConnectionLostFunction(int32 wiiRemoteId) override;
	virtual void OnBatteryChangedFunction(int32 wiiRemoteId, int32 batteryPercent) override;
	virtual void OnBatteryDrainedFunction(int32 wiiRemoteId) override;
	virtual void OnLEDsChangedFunction(int32 wiiRemoteId, WiiRemoteLED led) override;
	virtual void OnButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteButtons buttons) override;
	virtual void OnAccelChangedFunction(int32 wiiRemoteId, FVector accel) override;
	virtual void OnOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll) override;
	virtual void OnIRChangedFunction(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots) override;
	virtual void OnNunchukConnectedFunction(int32 wiiRemoteId) override;
	virtual void OnNunchukButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons) override;
	virtual void OnNunchukAccelChangedFunction(int32 wiiRemoteId, FVector accel) override;
	virtual void OnNunchukOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll) override;
	virtual void OnNunchukJoystickChangedFunction(int32 wiiRemoteId, float x, float y) override;
	virtual void OnClassicConnectedFunction(int32 wiiRemoteId) override;
	virtual void OnClassicButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons) override;
	virtual void OnClassicJoystickLChangedFunction(int32 wiiRemoteId, float x, float y) override;
	virtual void OnClassicJoystickRChangedFunction(int32 wiiRemoteId, float x, float y) override;
	virtual void OnClassicTriggersChangedFunction(int32 wiiRemoteId, float left, float right) override;
	virtual void OnBalanceConnectedFunction(int32 wiiRemoteId) override;
	virtual void OnBalanceWeightChangedFunction(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard) override;
	virtual void OnMotionPlusDetectedFunction(int32 wiiRemoteId) override;
	virtual void OnMotionPlusEnabledFunction(int32 wiiRemoteId) override;
	virtual void OnMotionPlusSpeedChangedFunction(int32 wiiRemoteId, FRotator speed) override;
	virtual void OnMotionPlusExtensionConnectedFunction(int32 wiiRemoteId) override;
	virtual void OnMotionPlusExtensionDisconnectedFunction(int32 wiiRemoteId) override;
	virtual void OnExtensionDisconnectedFunction(int32 wiiRemoteId) override;
};
