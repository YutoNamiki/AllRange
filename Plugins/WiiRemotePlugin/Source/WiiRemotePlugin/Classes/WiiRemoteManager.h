#pragma once

#include "AllowWindowsPlatformTypes.h"
#include "WiiRemoteYourself.h"
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
	WiiRemote WiiRemotes[4];
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
	WiiRemote* LastValidWiiRemote();
	bool IsWiiRemoteValidForInputMapping(uint64 wiiRemoteId);
	int32 WiiRemoteIndexForWiiRemote(uint64 wiiRemoteId);

	void OnConnected(WiiRemote* wiiRemote);
	void OnConnectionLost(WiiRemote* wiiRemote);
	void OnBatteryChanged(WiiRemote* wiiRemote, uint8 batteryPercent);
	void OnBatteryDrained(WiiRemote* wiiRemote);
	void OnLEDsChanged(WiiRemote* wiiRemote, uint8 ledBits);
	void OnButtonsChanged(WiiRemote* wiiRemote, FWiiRemoteButtons buttons);
	void OnAccelChanged(WiiRemote* wiiRemote, FVector accel);
	void OnOrientationChanged(WiiRemote* wiiRemote, float pitch, float roll);
	void OnIRChanged(WiiRemote* wiiRemote, TArray<FWiiRemoteDot> dots);
	void OnNunchukConnected(WiiRemote* wiiRemote);
	void OnNunchukButtonsChanged(WiiRemote* wiiRemote, FWiiRemoteNunchukButtons buttons);
	void OnNunchukAccelChanged(WiiRemote* wiiRemote, FVector accel);
	void OnNunchukOrientationChanged(WiiRemote* wiiRemote, float pitch, float roll);
	void OnNunchukJoystickChanged(WiiRemote* wiiRemote, float x, float y);
	void OnClassicConnected(WiiRemote* wiiRemote);
	void OnClassicButtonsChanged(WiiRemote* wiiRemote, FWiiRemoteClassicControllerButtons buttons);
	void OnClassicJoystickLChanged(WiiRemote* wiiRemote, float x, float y);
	void OnClassicJoystickRChanged(WiiRemote* wiiRemote, float x, float y);
	void OnClassicTriggersChanged(WiiRemote* wiiRemote, float left, float right);
	void OnBalanceConnected(WiiRemote* wiiRemote);
	void OnBalanceWeightChanged(WiiRemote* wiiRemote, FWiiRemoteBalanceBoard balanceBoard);
	void OnMotionPlusDetected(WiiRemote* wiiRemote);
	void OnMotionPlusEnabled(WiiRemote* wiiRemote);
	void OnMotionPlusSpeedChanged(WiiRemote* wiiRemote, FRotator speed);
	void OnMotionPlusExtensionConnected(WiiRemote* wiiRemote);
	void OnMotionPlusExtensionDisconnected(WiiRemote* wiiRemote);
	void OnExtensionDisconnected(WiiRemote* wiiRemote);
	static void SetWiiRemoteButtons(WiiRemote& wiiRemote, FWiiRemoteButtons& buttons);
	static void SetWiiRemoteDots(WiiRemote& wiiRemote, TArray<FWiiRemoteDot>& dots);
	static void SetWiiRemoteNunchukButtons(WiiRemote& wiiRemote, FWiiRemoteNunchukButtons& buttons);
	static void SetWiiRemoteClassicControllerButtons(WiiRemote& wiiRemote, FWiiRemoteClassicControllerButtons& buttons);
	static void SetWiiRemoteBalanceBoard(WiiRemote& wiiRemote, FWiiRemoteBalanceBoard& balanceBoard);
	
private:
	static bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat);
	static bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat);
	static bool EmitAnalogInputEventForKey(FKey key, float value, int32 user);
	static FRotator ConvertOrientationToUE(FRotator rawOrientation);
	static FVector ConvertVectorToUE(FVector rawAcceleration);
	static void UpdateButtons(bool oldState, bool newState, FKey key, int32 user);
};
