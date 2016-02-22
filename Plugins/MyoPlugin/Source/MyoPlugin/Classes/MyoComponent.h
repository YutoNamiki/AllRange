#pragma once

#include "MyoDelegate.h"
#include "MyoComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = "Input Controller", meta = (BlueprintSpawnableComponent))
class MYOPLUGIN_API UMyoComponent : public UActorComponent, public IMyoDelegate
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FConnectSignature OnConnect;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FDisconnectSignature OnDisconnect;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FPairSignature OnPair;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FUnPairSignature OnUnpair;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FArmMovedSignature OnArmMoved;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FQuaternionDataSignature OnQuaternionData;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FOrientationDataSignature OnOrientationData;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FAccelerometerDataSignature OnAccelerometerData;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FGyroscopeDataSignature OnGyroscopeData;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FPoseSignature OnPose;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FArmSyncSignature OnArmSync;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FArmUnsyncSignature OnArmUnsync;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FEmgDataSignature OnEmgData;
	UPROPERTY(BlueprintAssignable, Category = "Myo Armband")
	FDisabledSignature OnDisabled;

	UMyoComponent(const FObjectInitializer& init);
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	bool IsHubEnabled();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void SetLockingPolicy(MyoLockingPolicy policy);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	UMyoController* LeftMyo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	UMyoController* RightMyo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	UMyoController* PrimaryMyo();
	UFUNCTION(BlueprintCallable, Category = MyoUtilityFunctions)
	void ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted);
	UFUNCTION(BlueprintCallable, Category = MyoUtilityFunctions)
	void VibrateDevice(int32 playerIndex, MyoVibrationType type);

	virtual void OnConnectFunction(int32 myoId) override;
	virtual void OnDisconnectFunction(int32 myoId) override;
	virtual void OnPairFunction(int32 myoId) override;
	virtual void OnUnpairFunction(int32 myoId) override;
	virtual void OnArmMovedFunction(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose) override;
	virtual void OnOrientationDataFunction(int32 myoId, FQuat quat) override;
	virtual void OnOrientationDataFunction(int32 myoId, FRotator rot) override;
	virtual void OnAccelerometerDataFunction(int32 myoId, FVector accel) override;
	virtual void OnGyroscopeDataFunction(int32 myoId, FVector gyro) override;
	virtual void OnPoseFunction(int32 myoId, MyoPose pose) override;
	virtual void OnArmSyncFunction(int32 myoId, MyoArm arm, MyoArmDirection direction) override;
	virtual void OnArmUnsyncFunction(int32 myoId) override;
	virtual void OnEmgDataFunction(int32 myoId, FMyoEmgData data) override;
	virtual void MyoDisabled() override;

};
