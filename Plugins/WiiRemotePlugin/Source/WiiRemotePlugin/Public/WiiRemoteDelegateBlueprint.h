#pragma once

#include "WiiRemoteDelegate.h"
#include "WiiRemoteDelegateBlueprint.generated.h"

class UWiiRemoteController;

UINTERFACE(MinimalAPI)
class UWiiRemoteDelegateBlueprint : public UWiiRemoteDelegate
{
	GENERATED_UINTERFACE_BODY()
};

class IWiiRemoteDelegateBlueprint : public IWiiRemoteDelegate
{
	GENERATED_IINTERFACE_BODY()

public:
	UObject* ValidSelfPointer;
	UObject* InterfaceDelegate;
	TArray <UWiiRemoteController*> LatestFrame;

	void SetInterfaceDelegate(UObject* newDelegate);
	bool IsValidDelegate();
	UWiiRemoteController* InternalAddController(int32 newId);
	UWiiRemoteController* InternalControllerForId(int32 newId);
	virtual void OnConnected(int32 wiiRemoteId) override;
	virtual void OnCennectionLost(int32 wiiRemoteId) override;
	virtual void OnBatteryChanged(int32 wiiRemoteId, int32 batteryPercent) override;
	virtual void OnBatteryDrained(int32 wiiRemoteId) override;
	virtual void OnLEDsChanged(int32 wiiRemoteId, WiiRemoteLED led) override;
	virtual void OnButtonsChanged(int32 wiiRemoteId, FWiiRemoteButtons buttons) override;
	virtual void OnAccelChanged(int32 wiiRemoteId, FVector accel) override;
	virtual void OnOrientationChanged(int32 wiiRemoteId, float pitch, float roll) override;
	virtual void OnIRChanged(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots) override;
	virtual void OnNunchukConnected(int32 wiiRemoteId) override;
	virtual void OnNunchukButtonsChanged(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons) override;
	virtual void OnNunchukAccelChanged(int32 wiiRemoteId, FVector accel) override;
	virtual void OnNunchukOrientationChanged(int32 wiiRemoteId, float pitch, float roll) override;
	virtual void OnNunchukJoystickChanged(int32 wiiRemoteId, float x, float y) override;
	virtual void OnClassicConnected(int32 wiiRemoteId) override;
	virtual void OnClassicButtonsChanged(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons) override;
	virtual void OnClassicJoystickLChanged(int32 wiiRemoteId, float x, float y) override;
	virtual void OnClassicJoystickRChanged(int32 wiiRemoteId, float x, float y) override;
	virtual void OnClassicTriggersChanged(int32 wiiRemoteId, float left, float right) override;
	virtual void OnBalanceConnected(int32 wiiRemoteId) override;
	virtual void OnBalanceWeightChanged(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard) override;
	virtual void OnMotionPlusDetected(int32 wiiRemoteId) override;
	virtual void OnMotionPlusEnabled(int32 wiiRemoteId) override;
	virtual void OnMotionPlusSpeedChanged(int32 wiiRemoteId, FRotator speed) override;
	virtual void OnMotionPlusExtensionConnected(int32 wiiRemoteId) override;
	virtual void OnMotionPlusExtensionDisconnected(int32 wiiRemoteId) override;
	virtual void OnExtensionDisconnected(int32 wiiRemoteId) override;
	virtual bool IsWiiRemoteValidId(int32 wiiRemoteId) override;
	virtual void Startup() override;
	virtual void Shutdown() override;
	virtual void Tick(float deltaTime) override;

};
