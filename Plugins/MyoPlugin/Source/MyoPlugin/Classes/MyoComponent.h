#pragma once

#include "MyoDelegateBlueprint.h"
#include "MyoComponent.generated.h"

class UMyoController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyoOnConnectDelegate, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyoOnDisconnectDelegate, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyoOnPairDelegate, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyoOnUnpairDelegate, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FMyoOnArmMovedDelegate, UMyoController*, Myo, FVector, armAcceleration, FRotator, armOrientation, FVector, armGyro, MyoPose, pose);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMyoOnOrientationDataDelegate, UMyoController*, myo, FRotator, orientation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMyoOnAccelerometerDataDelegate, UMyoController*, myo, FVector, acceleration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMyoOnGyroscopeDataDelegate, UMyoController*, myo, FVector, gyro);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMyoOnPoseDelegate, UMyoController*, myo, MyoPose, pose);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMyoOnArmSyncDelegate, UMyoController*, myo, MyoArm, arm, MyoArmDirection, direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMyoOnArmUnsyncDelegate, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMyoOnEmgDataDelegate, UMyoController*, myo, FMyoEmgData, data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyoDeviceDisabledDelegate);

UCLASS(Blueprintable, ClassGroup = "Input Controller", meta = (BlueprintSpawnableComponent))
class MYOPLUGIN_API UMyoComponent : public UActorComponent, public IMyoDelegateBlueprint
{
	GENERATED_BODY()

public:
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

	virtual void OnConnect(int32 myoId) override;
	virtual void OnDisconnect(int32 myoId) override;
	virtual void OnPair(int32 myoId) override;
	virtual void OnUnpair(int32 myoId) override;
	virtual void OnArmMoved(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose);
	virtual void OnOrientationData(int32 myoId, FRotator rot) override;
	virtual void OnAccelerometerData(int32 myoId, FVector accel) override;
	virtual void OnGyroscopeData(int32 myoId, FVector gyro) override;
	virtual void OnPose(int32 myoId, MyoPose pose) override;
	virtual void OnArmSync(int32 myoId, MyoArm arm, MyoArmDirection direction) override;
	virtual void OnArmUnsync(int32 myoId) override;
	virtual void OnEmgData(int32 myoId, FMyoEmgData data) override;
	virtual void MyoDisabled() override;

	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnConnectDelegate MyoOnConnect;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnDisconnectDelegate MyoOnDisconnect;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnPairDelegate MyoOnPair;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnUnpairDelegate MyoOnUnpair;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnArmMovedDelegate MyoOnArmMoved;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnOrientationDataDelegate MyoOnOrientationData;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnAccelerometerDataDelegate MyoOnAccelerometerData;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnGyroscopeDataDelegate MyoOnGyroscopeData;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnPoseDelegate MyoOnPose;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnArmSyncDelegate MyoOnArmSync;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnArmUnsyncDelegate MyoOnArmUnsync;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoOnEmgDataDelegate MyoOnEmgData;
	UPROPERTY(BlueprintAssignable, Category = "Myo")
	FMyoDeviceDisabledDelegate MyoDeviceDisabled;
};
