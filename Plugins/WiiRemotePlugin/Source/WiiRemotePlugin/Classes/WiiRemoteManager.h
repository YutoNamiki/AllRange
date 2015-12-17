#pragma once

#include "AllowWindowsPlatformTypes.h"
#include "wiimote.h"
#include "HideWindowsPlatformTypes.h"

#include "WiiRemoteManager.generated.h"

struct FWiiRemoteButtons;
struct FWiiRemoteDot;
struct FWiiRemoteNunchukButtons;
struct FWiiRemoteClassicControllerButtons;
struct FWiiRemoteBalanceBoard;
struct FWiiRemoteDeviceData;
class IWiiRemoteDelegate;

USTRUCT(BlueprintType)
struct FWiiRemoteChangeFlags
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsConnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsConnectionLost = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsBatteryChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsBatteryDrained = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsLEDChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsNunchukConnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsClassicConnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsBalanceConnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMotionPlusDetected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMotionPlusEnabled = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMotionPlusExtensionConnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMotionPlusExtensionDisconnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsExtensionDisconnected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsButtonsChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAccelChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOrientationChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsIRChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsNunchukButtonsChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsNunchukAccelChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsNunchukOrientationChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsNunchukJoystickChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsClassicButtonsChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsClassicJoystickLChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsClassicJoystickRChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsClassicTriggerChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsBalanceWeightChanged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsMotionPlusSpeedChanged = false;
};

UCLASS()
class WIIREMOTEPLUGIN_API UWiiRemoteManager : public UObject
{
	GENERATED_BODY()

public:
	//TArray<wiimote> WiiRemotes;
	wiimote WiiRemotes[4];
	TArray<FWiiRemoteDeviceData> Data;
	TArray<FWiiRemoteChangeFlags> ChangeFlags;
	TArray<uint64> LastPairedWiiRemoteID;
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
	static bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat);
	static bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat);
	static bool EmitAnalogInputEventForKey(FKey key, float value, int32 user);
	static FRotator ConvertOrientationToUE(FRotator rawOrientation);
	static FVector ConvertVectorToUE(FVector rawAcceleration);
	static void UpdateButtons(bool oldState, bool newState, FKey key, int32 user);
};
