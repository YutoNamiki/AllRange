#pragma once

#include "MyoDelegate.h"
#include "MyoInterface.generated.h"

class UMyoController;

UINTERFACE(MinimalAPI)
class UMyoInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IMyoInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnConnect(UMyoController* myo);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnDisconnect(UMyoController* myo);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnPair(UMyoController* myo);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnUnpair(UMyoController* myo);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnArmMoved(UMyoController* myo, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnOrientationData(UMyoController* myo, FRotator orientation);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnAccelerometerData(UMyoController* myo, FVector acceleration);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnGyroscopeData(UMyoController* myo, FVector gyro);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnPose(UMyoController* myo, MyoPose pose);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnArmSync(UMyoController* myo, MyoArm arm, MyoArmDirection direction);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnArmUnsync(UMyoController* myo);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void OnEmgData(UMyoController* myo, FMyoEmgData data);
	UFUNCTION(BlueprintImplementableEvent, Category = MyoEvents)
	void DeviceDisabled();

};
