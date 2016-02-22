#pragma once

#include "WiiRemoteDelegate.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(WiiRemotePluginLog, Log, All);

class UWiiRemoteController;

UENUM(BlueprintType)
enum class ReportType : uint8
{
	Buttons							UMETA(DisplayName = "Buttons"),
	ButtonsAcceleration				UMETA(DisplayName = "ButtonsAcceleration"),
	ButtonsAccelerationIR			UMETA(DisplayName = "ButtonsAccelerationIR"),
	ButtonsAccelerationExtension	UMETA(DisplayName = "ButtonsAccelerationExtension"),
	ButtonsAccelerationIRExtension	UMETA(DisplayName = "ButtonsAccelerationIRExtension"),
	ButtonsBalanceBoard				UMETA(DisplayName = "ButtonsBalanceBoard")
};

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsInitialized = false;
};

USTRUCT()
struct FWiiRemoteSample
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<uint8> Samples;
	UPROPERTY()
	int32 Length = 0;
	UPROPERTY()
	WiiRemoteSpeakerFrequency Frequency = WiiRemoteSpeakerFrequency::FreqNone;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectionLostSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBatteryChangedSignature, UWiiRemoteController*, WiiRemote, int32, BatteryPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBatteryDrainedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FLEDsChangedSignature, UWiiRemoteController*, WiiRemote, WiiRemoteLED, LED);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FButtonsChangedSignature, UWiiRemoteController*, WiiRemote, FWiiRemoteButtons, Buttons);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAccelChangedSignature, UWiiRemoteController*, WiiRemote, FVector, Accel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOrientationChangedSignature, UWiiRemoteController*, WiiRemote, float, Pitch, float, Roll);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIRChangedSignature, UWiiRemoteController*, WiiRemote, TArray<FWiiRemoteDot>, Dot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNunchukConnectedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNunchukButtonsChangedSignature, UWiiRemoteController*, WiiRemote, FWiiRemoteNunchukButtons, Buttons);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FNunchukAccelChangedSignature, UWiiRemoteController*, WiiRemote, FVector, Accel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNunchukOrientationChangedSignature, UWiiRemoteController*, WiiRemote, float, Pitch, float, Roll);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FNunchukJoystickChangedSignature, UWiiRemoteController*, WiiRemote, float, X, float, Y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FClassicConnectedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FClassicButtonsChangedSignature, UWiiRemoteController*, WiiRemote, FWiiRemoteClassicControllerButtons, Buttons);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FClassicJoystickLChangedSignature, UWiiRemoteController*, WiiRemote, float, X, float, Y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FClassicJoystickRChangedSignature, UWiiRemoteController*, WiiRemote, float, X, float, Y);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FClassicTriggersChangedSignature, UWiiRemoteController*, WiiRemote, float, Left, float, Right);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBalanceConnectedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBalanceWeightChangedSignature, UWiiRemoteController*, WiiRemote, FWiiRemoteBalanceBoard, BalanceBoard);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMotionPlusDetectedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMotionPlusEnabledSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMotionPlusSpeedChangedSignature, UWiiRemoteController*, WiiRemote, FRotator, Speed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMotionPlusExtensionConnectedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMotionPlusExtensionDisconnectedSignature, UWiiRemoteController*, WiiRemote);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FExtensionDisconnectedSignature, UWiiRemoteController*, WiiRemote);

UINTERFACE(MinimalAPI)
class UWiiRemoteDelegate : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IWiiRemoteDelegate
{
	GENERATED_IINTERFACE_BODY()

public:
	UObject* ValidSelfPointer;
	UObject* InterfaceDelegate;
	TArray<UWiiRemoteController*> LatestFrame;

	void SetInterfaceDelegate(UObject* newDelegate);
	bool IsValidDelegate();
	UWiiRemoteController* InternalAddController(int32 newId);
	UWiiRemoteController* InternalControllerForId(int32 newId);
	virtual void OnConnectedFunction(int32 wiiRemoteId);
	virtual void OnConnectionLostFunction(int32 wiiRemoteId);
	virtual void OnBatteryChangedFunction(int32 wiiRemoteId, int32 batteryPercent);
	virtual void OnBatteryDrainedFunction(int32 wiiRemoteId);
	virtual void OnLEDsChangedFunction(int32 wiiRemoteId, WiiRemoteLED led);
	virtual void OnButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteButtons buttons);
	virtual void OnAccelChangedFunction(int32 wiiRemoteId, FVector accel);
	virtual void OnOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll);
	virtual void OnIRChangedFunction(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots);
	virtual void OnNunchukConnectedFunction(int32 wiiRemoteId);
	virtual void OnNunchukButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons);
	virtual void OnNunchukAccelChangedFunction(int32 wiiRemoteId, FVector accel);
	virtual void OnNunchukOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll);
	virtual void OnNunchukJoystickChangedFunction(int32 wiiRemoteId, float x, float y);
	virtual void OnClassicConnectedFunction(int32 wiiRemoteId);
	virtual void OnClassicButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons);
	virtual void OnClassicJoystickLChangedFunction(int32 wiiRemoteId, float x, float y);
	virtual void OnClassicJoystickRChangedFunction(int32 wiiRemoteId, float x, float y);
	virtual void OnClassicTriggersChangedFunction(int32 wiiRemoteId, float left, float right);
	virtual void OnBalanceConnectedFunction(int32 wiiRemoteId);
	virtual void OnBalanceWeightChangedFunction(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard);
	virtual void OnMotionPlusDetectedFunction(int32 wiiRemoteId);
	virtual void OnMotionPlusEnabledFunction(int32 wiiRemoteId);
	virtual void OnMotionPlusSpeedChangedFunction(int32 wiiRemoteId, FRotator speed);
	virtual void OnMotionPlusExtensionConnectedFunction(int32 wiiRemoteId);
	virtual void OnMotionPlusExtensionDisconnectedFunction(int32 wiiRemoteId);
	virtual void OnExtensionDisconnectedFunction(int32 wiiRemoteId);
	virtual void SetReportType(int32 playerIndex, ReportType reportType);
	virtual void SetLED(int32 playerIndex, WiiRemoteLED ledBits);
	virtual void SetRumble(int32 playerIndex, bool on);
	virtual void SetRumbleForAsync(int32 playerIndex, int32 milliseconds);
	virtual void EnableSpeaker(int32 playerIndex, bool on);
	virtual void PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequency, int32 volume);
	virtual void PlaySample(int32 playerIndex, FWiiRemoteSample sample, WiiRemoteSpeakerFrequency frequency, int32 volume);
	virtual FWiiRemoteDeviceData* WiiRemoteLatestData(int32 wiiRemoteId);
	virtual int32 WiiRemoteMaxId();
	virtual bool IsWiiRemoteValidId(int32 wiiRemoteId);
	virtual void Startup();
	virtual void Shutdown();
	virtual void Tick(float deltaTime);
};
