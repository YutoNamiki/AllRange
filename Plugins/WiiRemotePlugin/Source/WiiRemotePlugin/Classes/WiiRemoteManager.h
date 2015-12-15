#pragma once

#include "WiiRemoteManager.generated.h"

class wiimote;
enum state_change_flags;
struct wiimote_state;
struct FWiiRemoteButtons;
struct FWiiRemoteDot;
struct FWiiRemoteNunchukButtons;
struct FWiiRemoteClassicControllerButtons;
struct FWiiRemoteBalanceBoard;
struct FWiiRemoteDeviceData;
class IWiiRemoteDelegate;

UCLASS()
class WIIREMOTEPLUGIN_API UWiiRemoteManager : public UObject
{
	GENERATED_BODY()

public:
	TArray<wiimote*> WiiRemotes;
	TArray<FWiiRemoteDeviceData> Data;
	IWiiRemoteDelegate* WiiRemoteDelegate;

	UWiiRemoteManager(class FObjectInitializer const& objectInitializer);
	~UWiiRemoteManager();

	bool Startup();
	void ShutDown();
	void Reset();
	void Tick(float deltaTime);
	int32 IdentifyWiiRemote(uint64 wiiRemoteId);
	uint64 LastValidWiiRemote();
	bool IsWiiRemoteValidForInputMapping(uint64 wiiRemoteId);
	int32 WiiRemoteIndexForWiiRemote(uint64 wiiRemoteId);

	void OnConnected(uint64 id);
	void OnConnectionLost(uint64 id);
	void OnBatteryChanged(uint64 id, uint8 batteryPercent);
	void OnBatteryDrained(uint64 id);
	void OnLEDsChanged(uint64 id, uint8 ledBits);
	void OnButtonsChanged(uint64 id, FWiiRemoteButtons buttons);
	void OnAccelChanged(uint64 id, FVector accel);
	void OnOrientationChanged(uint64 id, float pitch, float roll);
	void OnIRChanged(uint64 id, TArray<FWiiRemoteDot> dots);
	void OnNunchukConnected(uint64 id);
	void OnNunchukButtonsChanged(uint64 id, FWiiRemoteNunchukButtons buttons);
	void OnNunchukAccelChanged(uint64 id, FVector accel);
	void OnNunchukOrientationChanged(uint64 id, float pitch, float roll);
	void OnNunchukJoystickChanged(uint64 id, float x, float y);
	void OnClassicConnected(uint64 id);
	void OnClassicButtonsChanged(uint64 id, FWiiRemoteClassicControllerButtons buttons);
	void OnClassicJoystickLChanged(uint64 id, float x, float y);
	void OnClassicJoystickRChanged(uint64 id, float x, float y);
	void OnClassicTriggersChanged(uint64 id, float left, float right);
	void OnBalanceConnected(uint64 id);
	void OnBalanceWeightChanged(uint64 id, FWiiRemoteBalanceBoard balanceBoard);
	void OnMotionPlusDetected(uint64 id);
	void OnMotionPlusEnabled(uint64 id);
	void OnMotionPlusSpeedChanged(uint64 id, FRotator speed);
	void OnMotionPlusExtensionConnected(uint64 id);
	void OnMotionPlusExtensionDisconnected(uint64 id);
	void OnExtensionDisconnected(uint64 id);

private:
	void OnStateChanged(wiimote& wiiRemote, state_change_flags changed, wiimote_state& new_state);
 	static void SetWiiRemoteButtons(wiimote& wiiRemote, FWiiRemoteButtons& buttons);
	static void SetWiiRemoteDots(wiimote& wiiRemote, TArray<FWiiRemoteDot>& dots);
	static void SetWiiRemoteNunchukButtons(wiimote& wiiRemote, FWiiRemoteNunchukButtons& buttons);
	static void SetWiiRemoteClassicControllerButtons(wiimote& wiiRemote, FWiiRemoteClassicControllerButtons& buttons);
	static void SetWiiRemoteBalanceBoard(wiimote& wiiRemote, FWiiRemoteBalanceBoard& balanceBoard);

};
