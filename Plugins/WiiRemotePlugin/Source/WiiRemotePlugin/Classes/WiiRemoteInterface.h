#pragma once

#include "WiiRemoteDelegate.h"
#include "WiiRemoteInterface.generated.h"

class UWiiRemoteController;

UINTERFACE(MinimalAPI)
class UWiiRemoteInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IWiiRemoteInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnConnected(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnConnectionLost(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnBatteryChanged(UWiiRemoteController* wiiRemote, int32 batteryPercent);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnBatteryDrained(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnLEDsChanged(UWiiRemoteController* wiiRemote, WiiRemoteLED led);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnButtonsChanged(UWiiRemoteController* wiiRemote, FWiiRemoteButtons buttons);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnAccelChanged(UWiiRemoteController* wiiRemote, FVector accel);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnOrientationChanged(UWiiRemoteController* wiiRemote, float pitch, float roll);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnIRChanged(UWiiRemoteController* wiiRemote, const TArray<FWiiRemoteDot>& dots);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnNunchukConnected(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnNunchukButtonsChanged(UWiiRemoteController* wiiRemote, FWiiRemoteNunchukButtons buttons);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnNunchukAccelChanged(UWiiRemoteController* wiiRemote, FVector accel);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnNunchukOrientationChanged(UWiiRemoteController* wiiRemote, float pitch, float roll);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnNunchukJoystickChanged(UWiiRemoteController* wiiRemote, float x, float y);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnClassicConnected(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnClassicButtonsChanged(UWiiRemoteController* wiiRemote, FWiiRemoteClassicControllerButtons buttons);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnClassicJoystickLChanged(UWiiRemoteController* wiiRemote, float x, float y);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnClassicJoystickRChanged(UWiiRemoteController* wiiRemote, float x, float y);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnClassicTriggersChanged(UWiiRemoteController* wiiRemote, float left, float right);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnBalanceConnected(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnBalanceWeightChanged(UWiiRemoteController* wiiRemote, FWiiRemoteBalanceBoard balanceBoard);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnMotionPlusDetected(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnMotionPlusEnabled(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnMotionPlusSpeedChanged(UWiiRemoteController* wiiRemote, FRotator speed);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnMotionPlusExtensionConnected(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnMotionPlusExtensionDisconnected(UWiiRemoteController* wiiRemote);
	UFUNCTION(BlueprintImplementableEvent, Category = WiiRemoteEvents)
	void OnExtensionDisconnected(UWiiRemoteController* wiiRemote);

};
