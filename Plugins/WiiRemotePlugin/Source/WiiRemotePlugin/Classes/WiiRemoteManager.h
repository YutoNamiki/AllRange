#pragma once

#include "HAL/ThreadingBase.h"
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

class WiiRemoteConnectionWorker : public FRunnable
{
public:
	WiiRemoteConnectionWorker(class UWiiRemoteManager& wiiRemoteManager, TArray<wiimote*>& wiiRemotes);
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FThreadSafeCounter stopTaskCounter;
	UWiiRemoteManager* wiiRemoteManager;
	TArray<wiimote*>* wiiRemotes;
};

UCLASS()
class WIIREMOTEPLUGIN_API UWiiRemoteManager : public UObject
{
	GENERATED_BODY()

public:
	TArray<wiimote*> WiiRemotes;
	TArray<FWiiRemoteDeviceData> Data;
	uint64 LastPairedWiiRemoteID;
	IWiiRemoteDelegate* WiiRemoteDelegate;

	UWiiRemoteManager(class FObjectInitializer const& objectInitializer);
	~UWiiRemoteManager();

	bool Startup();
	void ShutDown();
	void Reset();
	void Tick(float deltaTime);
	int32 IdentifyWiiRemote(uint64 wiiRemoteId);
	wiimote* LastValidWiiRemote();
	bool IsWiiRemoteValidForInputMapping(uint64 wiiRemoteId);
	int32 WiiRemoteIndexForWiiRemote(uint64 wiiRemoteId);

	void OnConnected(wiimote* wiiRemote);
	void OnConnectionLost(wiimote* wiiRemote);
	void OnBatteryChanged(wiimote* wiiRemote, uint8 batteryPercent);
	void OnBatteryDrained(wiimote* wiiRemote);
	void OnLEDsChanged(wiimote* wiiRemote, uint8 ledBits);
	void OnButtonsChanged(wiimote* wiiRemote, FWiiRemoteButtons buttons);
	void OnAccelChanged(wiimote* wiiRemote, FVector accel);
	void OnOrientationChanged(wiimote* wiiRemote, float pitch, float roll);
	void OnIRChanged(wiimote* wiiRemote, TArray<FWiiRemoteDot> dots);
	void OnNunchukConnected(wiimote* wiiRemote);
	void OnNunchukButtonsChanged(wiimote* wiiRemote, FWiiRemoteNunchukButtons buttons);
	void OnNunchukAccelChanged(wiimote* wiiRemote, FVector accel);
	void OnNunchukOrientationChanged(wiimote* wiiRemote, float pitch, float roll);
	void OnNunchukJoystickChanged(wiimote* wiiRemote, float x, float y);
	void OnClassicConnected(wiimote* wiiRemote);
	void OnClassicButtonsChanged(wiimote* wiiRemote, FWiiRemoteClassicControllerButtons buttons);
	void OnClassicJoystickLChanged(wiimote* wiiRemote, float x, float y);
	void OnClassicJoystickRChanged(wiimote* wiiRemote, float x, float y);
	void OnClassicTriggersChanged(wiimote* wiiRemote, float left, float right);
	void OnBalanceConnected(wiimote* wiiRemote);
	void OnBalanceWeightChanged(wiimote* wiiRemote, FWiiRemoteBalanceBoard balanceBoard);
	void OnMotionPlusDetected(wiimote* wiiRemote);
	void OnMotionPlusEnabled(wiimote* wiiRemote);
	void OnMotionPlusSpeedChanged(wiimote* wiiRemote, FRotator speed);
	void OnMotionPlusExtensionConnected(wiimote* wiiRemote);
	void OnMotionPlusExtensionDisconnected(wiimote* wiiRemote);
	void OnExtensionDisconnected(wiimote* wiiRemote);
	static void SetWiiRemoteButtons(wiimote& wiiRemote, FWiiRemoteButtons& buttons);
	static void SetWiiRemoteDots(wiimote& wiiRemote, TArray<FWiiRemoteDot>& dots);
	static void SetWiiRemoteNunchukButtons(wiimote& wiiRemote, FWiiRemoteNunchukButtons& buttons);
	static void SetWiiRemoteClassicControllerButtons(wiimote& wiiRemote, FWiiRemoteClassicControllerButtons& buttons);
	static void SetWiiRemoteBalanceBoard(wiimote& wiiRemote, FWiiRemoteBalanceBoard& balanceBoard);
	
private:
	FRunnableThread* connectionThread;
	WiiRemoteConnectionWorker* connectionWorker;
	
	static bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat);
	static bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat);
	static bool EmitAnalogInputEventForKey(FKey key, float value, int32 user);
	static FRotator ConvertOrientationToUE(FRotator rawOrientation);
	static FVector ConvertVectorToUE(FVector rawAcceleration);
	static void UpdateButtons(bool oldState, bool newState, FKey key, int32 user);
};
