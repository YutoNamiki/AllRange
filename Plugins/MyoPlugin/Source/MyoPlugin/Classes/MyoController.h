#pragma once

#include "MyoDelegate.h"
#include "MyoController.generated.h"

UCLASS(BlueprintType)
class MYOPLUGIN_API UMyoController : public UObject
{
	friend class IMyoDelegate;

	GENERATED_BODY()

public:
	UMyoController(const FObjectInitializer &init);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	TEnumAsByte<MyoPose> Pose;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FVector Acceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FRotator Orientation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FVector Gyro;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	TEnumAsByte<MyoArm> Arm;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	TEnumAsByte<MyoArmDirection> XDirection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FVector ArmAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FRotator ArmOrientation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FVector ArmGyro;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FRotator ArmSpaceCorrection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	FVector BodySpaceNullAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Myo Frame")
	int32 MyoId;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Myo Frame")
	bool isOnLeftArm();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Myo Frame")
	bool isOnRightArm();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void VibrateDevice(MyoVibrationType type);
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void CalibrateArmOrientation(FRotator direction);
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void Unlock(MyoUnlockType type);
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void Lock();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void SetStreamEmg(MyoStreamEmgType streamType);

	void SetFromMyoDeviceData(FMyoDeviceData* data);

private:
	IMyoDelegate* myoDelegate;
};
