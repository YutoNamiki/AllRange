#pragma once

#include "WiiRemoteDelegate.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(WiiRemotePluginLog, Log, All);

UENUM(BlueprintType)
enum class SpeakerFrequency : uint8
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
	FRotator PlusSpeed = FRotator::ZeroRotator;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	int32 LED;
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

	virtual void OnConnected();
	virtual void OnCennectionLost();
	virtual void OnBatteryChanged();
	virtual void OnBatteryDrained();
	virtual void OnLEDsChanged();
	virtual void OnButtonsChanged();
	virtual void OnAccelChanged();
	virtual void OnOrientationChanged();
	virtual void OnIRChanged();
	virtual void OnNunchukConnected();
	virtual void OnNunchukButtonsChanged();
	virtual void OnNunchukAccelChanged();
	virtual void OnNunchukOrientationChanged();
	virtual void OnNunchukJoystickChanged();
	virtual void OnClassicConnected();
	virtual void OnClassicButtonsChanged();
	virtual void OnClassicJoystickLChanged();
	virtual void OnClassicJoystickRChanged();
	virtual void OnClassicTriggersChanged();
	virtual void OnBalanceConnected();
	virtual void OnBalanceWeightChanged();
	virtual void OnMotionPlusDetected();
	virtual void OnMotionPlusEnabled();
	virtual void OnMotionPlusSpeedChanged();
	virtual void OnMotionPlusExtensionConnected();
	virtual void OnMotionPlusExtensionDisconnected();
	virtual void OnExtensionDisconnected();
	virtual void SetLED(unsigned char ledBits);
	virtual void SetRumble(bool on);
	virtual void SetRumbleForAsync(unsigned int milliseconds);
	virtual void MuteSpeaker(bool on);
	virtual void PlaySquareWave(SpeakerFrequency frequency, unsigned char volume);
	virtual void PlaySample(SpeakerFrequency frequency, unsigned char volume);
	virtual void Tick(float deltaTime);
};
