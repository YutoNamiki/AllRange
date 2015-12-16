#pragma once

#include "WiiRemoteDelegate.h"
#include "WiiRemoteDelegateBlueprint.h"
#include "WiiRemoteController.generated.h"

UCLASS(BlueprintType)
class WIIREMOTEPLUGIN_API UWiiRemoteController : public UObject
{
	friend class IWiiRemoteDelegateBlueprint;

	GENERATED_BODY()

public:
	UWiiRemoteController(const FObjectInitializer& init);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	int32 BatteryPercent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	FWiiRemoteButtons Buttons;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	TArray<FWiiRemoteDot> Dots;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	FVector Acceleration = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float OrientationPitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float OrientationRoll = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	FWiiRemoteNunchukButtons NunchukButtons;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	FVector NunchukAcceleration = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float NunchukOrientationPitch = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float NunchukOrientationRoll = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float NunchukJoystickX = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float NunchukJoystickY = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	FWiiRemoteClassicControllerButtons ClassicControllerButtons;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float ClassicControllerLeftJoystickX = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float ClassicControllerLeftJoystickY = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float ClassicControllerRightJoystickX = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float ClassicControllerRightJoystickY = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float ClassicControllerRightTrigger = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	float ClassicControllerLeftTrigger = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	FWiiRemoteBalanceBoard BalanceBoard;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	FRotator MotionPlusSpeed = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	TEnumAsByte<WiiRemoteLED> LED;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WiiRemote Frame")
	int32 WiiRemoteID = 0;

	void SetFromWiiRemoteDevciceData(FWiiRemoteDeviceData* data);

private:
	IWiiRemoteDelegate* wiiRemoteDelegate;
};
