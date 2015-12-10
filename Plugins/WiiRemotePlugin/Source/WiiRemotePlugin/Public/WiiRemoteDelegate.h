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
struct FWiiRemoteDeviceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WiiRemoteInput")
	bool IsPushA = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="WiiRemoteInput")
	bool IsPushB = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushPlus = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushHome = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushMinus = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushOne = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushTwo = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushUp = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushDown = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushLeft = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	bool IsPushRight = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FVector Acceleration = FVector::ZeroVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float OrientationPitch = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	float OrientationRoll = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WiiRemoteInput")
	FVector Gyro = FVector::ZeroVector;
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
	virtual void OnWiiRemoteChanged();
	virtual void OnNunchukConnected();
	virtual void OnNunchukButtonsChanged();
	virtual void OnNunchukAccelChanged();
	virtual void OnNunchukOrientationChanged();
	virtual void OnNunchukJoystickChanged();
	virtual void OnNunchukChanged();
	virtual void OnClassicConnected();
	virtual void OnClassicButtonsChanged();
	virtual void OnClassicJoystickLChanged();
	virtual void OnClassicJoystickRChanged();
	virtual void OnClassicTriggersChanged();
	virtual void OnClassicChanged();
	virtual void OnBalanceConnected();
	virtual void OnBalanceWeightChanged();
	virtual void OnBalanceChanged();
	virtual void OnMotionPlusDetected();
	virtual void OnMotionPlusEnabled();
	virtual void OnMotionPlusSpeedChanged();
	virtual void OnMotionPlusExtensionConnected();
	virtual void OnMotionPlusExtensionDisconnected();
	virtual void OnMotionPlusChanged();
	virtual void OnExtensionDisconnected();
	virtual void OnExtensionPartiallyInserted();
	virtual void OnExtensionConnected();
	virtual void OnExtensionChanged();
	virtual void ChangedAll();
	virtual void SetLED(unsigned char ledBits);
	virtual void SetRumble(bool on);
	virtual void SetRumbleForAsync(unsigned int milliseconds);
	virtual void MuteSpeaker(bool on);
	virtual void PlaySquareWave(SpeakerFrequency frequency, unsigned char volume);
	virtual void PlaySample(SpeakerFrequency frequency, unsigned char volume);
	virtual void Tick(float deltaTime);
};
