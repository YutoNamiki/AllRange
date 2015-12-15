#pragma once

#include "WiiRemoteManager.generated.h"

class wiimote;
enum state_change_flags;
struct wiimote_state;
struct FWiiRemoteButtons;

UCLASS()
class WIIREMOTEPLUGIN_API UWiiRemoteManager : public UObject
{
	GENERATED_BODY()

public:
	TArray<wiimote*> WiiRemotes;

	UWiiRemoteManager(class FObjectInitializer const& objectInitializer);
	~UWiiRemoteManager();
	void OnConnected(uint64 id);
	void OnConnectionLost(uint64 id);
	void OnBatteryChanged(uint64 id, uint8 batteryPercent);
	void OnBatteryDrained(uint64 id);
	void OnLEDsChanged(uint64 id, uint8 ledBits);
	void OnButtonsChanged(uint64 id, FWiiRemoteButtons buttons);
	void OnAccelChanged(uint64 id, FVector accel);
	void OnOrientationChanged(uint64 id, float pitch, float roll);
	void OnIRChanged();
	void OnWiiRemoteChanged();
	void OnNunchukConnected();
	void OnNunchukButtonsChanged();
	void OnNunchukAccelChanged();
	void OnNunchukOrientationChanged();
	void OnNunchukJoystickChanged();
	void OnNunchukChanged();
	void OnClassicConnected();
	void OnClassicButtonsChanged();
	void OnClassicJoystickLChanged();
	void OnClassicJoystickRChanged();
	void OnClassicTriggersChanged();
	void OnClassicChanged();
	void OnBalanceConnected();
	void OnBalanceWeightChanged();
	void OnBalanceChanged();
	void OnMotionPlusDetected();
	void OnMotionPlusEnabled();
	void OnMotionPlusSpeedChanged();
	void OnMotionPlusExtensionConnected();
	void OnMotionPlusExtensionDisconnected();
	void OnMotionPlusChanged();
	void OnExtensionDisconnected();
	void OnExtensionPartiallyInserted();
	void OnExtensionConnected();
	void OnExtensionChanged();
	void ChangedAll();

private:

	void OnStateChanged(wiimote& wiiRemote, state_change_flags changed, wiimote_state& new_state);

};
