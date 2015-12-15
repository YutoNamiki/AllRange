#pragma once

#include "WiiRemoteDelegate.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(WiiRemotePluginLog, Log, All);

UENUM(BlueprintType)
enum class WiiRemoteSpeakerFrequency : uint8
{
	FreqNone	UMETA(DisplayName = "FreqNone"),
	Freq4200Hz	UMETA(DisplayName = "Freq4200Hz"),
	Freq3920Hz	UMETA(DisplayName = "Freq3920Hz"),
	Freq3640Hz	UMETA(DisplayName = "Freq3640Hz"),
	Freq3360Hz	UMETA(DisplayName = "Freq3360Hz"),
	Freq3130Hz	UMETA(DisplayName = "Freq3130Hz"),
	Freq2940Hz	UMETA(DisplayName = "Freq2940Hz"),
	Freq2760Hz	UMETA(DisplayName = "Freq2760Hz"),
	Freq2610Hz	UMETA(DisplayName = "Freq2610Hz"),
	Freq2470Hz	UMETA(DisplayName = "Freq2470Hz")
};

UENUM(BlueprintType)
enum class WiiRemoteLED : uint8
{
	XXXX	UMETA(DisplayName = "XXXX"),
	XXXO	UMETA(DisplayName = "XXXO"),
	XXOX	UMETA(DisplayName = "XXOX"),
	XXOO	UMETA(DisplayName = "XXOO"),
	XOXX	UMETA(DisplayName = "XOXX"),
	XOXO	UMETA(DisplayName = "XOXO"),
	XOOX	UMETA(DisplayName = "XOOX"),
	XOOO	UMETA(DisplayName = "XOOO"),
	OXXX	UMETA(DisplayName = "OXXX"),
	OXXO	UMETA(DisplayName = "OXXO"),
	OXOX	UMETA(DisplayName = "OXOX"),
	OXOO	UMETA(DisplayName = "OXOO"),
	OOXX	UMETA(DisplayName = "OOXX"),
	OOXO	UMETA(DisplayName = "OOXO"),
	OOOO	UMETA(DisplayName = "OOOO")
};

USTRUCT(BlueprintType)
struct FWiiRemoteButtons
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushA = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushB = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushPlus = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushHome = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushMinus = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushOne = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushTwo = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushUp = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushDown = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushLeft = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteButtons")
	bool IsPushRight = false;
};

USTRUCT(BlueprintType)
struct FWiiRemoteDot
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteDot")
	bool bVisible;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteDot")
	float X;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteDot")
	float Y;
};

USTRUCT(BlueprintType)
struct FWiiRemoteNunchukButtons
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteNunchukButtons")
	bool IsPushC = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteNunchukButtons")
	bool IsPushZ = false;
};

USTRUCT(BlueprintType)
struct FWiiRemoteClassicControllerButtons
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushA = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushB = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushX = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushY = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushPlus = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushMinus = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushHome = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushUp = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushDown = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushRight = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushLeft = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushZL = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushZR = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushTriggerL = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteClassicControllerButtons")
	bool IsPushTriggerR = false;
};

USTRUCT(BlueprintType)
struct FWiiRemoteBalanceBoardSensor
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceSensor")
	float TopL = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceSensor")
	float TopR = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceSensor")
	float BottomL = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceSensor")
	float BottomR = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceSensor")
	float Total = 0.0f;
};

USTRUCT(BlueprintType)
struct FWiiRemoteBalanceBoard
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceBoard")
	FWiiRemoteBalanceBoardSensor AtRestKilograms;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceBoard")
	FWiiRemoteBalanceBoardSensor Kilograms;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteBalanceBoard")
	FWiiRemoteBalanceBoardSensor Pounds;
};

USTRUCT(BlueprintType)
struct FWiiRemoteDeviceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	int32 BatteryPercent = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FWiiRemoteButtons Buttons;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	TArray<FWiiRemoteDot> Dots;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FVector Acceleration = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float OrientationPitch = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float OrientationRoll = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FWiiRemoteNunchukButtons NunchukButtons;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FVector NunchukAcceleration = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float NunchukOrientationPitch = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float NunchukOrientationRoll = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float NunchukJoystickX = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float NunchukJoystickY = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FWiiRemoteClassicControllerButtons ClassicControllerButtons;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float ClassicControllerLeftJoystickX = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float ClassicControllerLeftJoystickY = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float ClassicControllerRightJoystickX = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float ClassicControllerRightJoystickY = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float ClassicControllerRightTrigger = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float ClassicControllerLeftTrigger = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FWiiRemoteBalanceBoard BalanceBoard;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FRotator MotionPlusSpeed = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	WiiRemoteLED LED;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsRumble = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsConnectNunchuk = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsConnectClassicController = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsConnectBalanceBoard = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsConnectMotionPlus = false;
};

UINTERFACE(MinimalAPI)
class UWiiRemoteDelegate : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IWiiRemoteDelegate
{
	GENERATED_IINTERFACE_BODY()

	virtual void OnConnected(int32 wiiRemoteId);
	virtual void OnCennectionLost(int32 wiiRemoteId);
	virtual void OnBatteryChanged(int32 wiiRemoteId, int32 batteryPercent);
	virtual void OnBatteryDrained(int32 wiiRemoteId);
	virtual void OnLEDsChanged(int32 wiiRemoteId, WiiRemoteLED led);
	virtual void OnButtonsChanged(int32 wiiRemoteId, FWiiRemoteButtons buttons);
	virtual void OnAccelChanged(int32 wiiRemoteId, FVector accel);
	virtual void OnOrientationChanged(int32 wiiRemoteId, float pitch, float roll);
	virtual void OnIRChanged(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots);
	virtual void OnNunchukConnected(int32 wiiRemoteId);
	virtual void OnNunchukButtonsChanged(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons);
	virtual void OnNunchukAccelChanged(int32 wiiRemoteId, FVector accel);
	virtual void OnNunchukOrientationChanged(int32 wiiRemoteId, float pitch, float roll);
	virtual void OnNunchukJoystickChanged(int32 wiiRemoteId, float x, float y);
	virtual void OnClassicConnected(int32 wiiRemoteId);
	virtual void OnClassicButtonsChanged(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons);
	virtual void OnClassicJoystickLChanged(int32 wiiRemoteId, float x, float y);
	virtual void OnClassicJoystickRChanged(int32 wiiRemoteId, float x, float y);
	virtual void OnClassicTriggersChanged(int32 wiiRemoteId, float left, float right);
	virtual void OnBalanceConnected(int32 wiiRemoteId);
	virtual void OnBalanceWeightChanged(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard);
	virtual void OnMotionPlusDetected(int32 wiiRemoteId);
	virtual void OnMotionPlusEnabled(int32 wiiRemoteId);
	virtual void OnMotionPlusSpeedChanged(int32 wiiRemoteId, FRotator speed);
	virtual void OnMotionPlusExtensionConnected(int32 wiiRemoteId);
	virtual void OnMotionPlusExtensionDisconnected(int32 wiiRemoteId);
	virtual void OnExtensionDisconnected(int32 wiiRemoteId);
	virtual void SetLED(unsigned char ledBits);
	virtual void SetRumble(bool on);
	virtual void SetRumbleForAsync(int32 milliseconds);
	virtual void MuteSpeaker(bool on);
	virtual void PlaySquareWave(WiiRemoteSpeakerFrequency frequency, int32 volume);
	virtual void PlaySample(WiiRemoteSpeakerFrequency frequency, int32 volume);
	virtual void Tick(float deltaTime);
};
