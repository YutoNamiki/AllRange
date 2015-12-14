#pragma once

#include "MyoDelegate.h"
#include "MyoDelegateBlueprint.generated.h"

class UMyoController;

UINTERFACE(MinimalAPI)
class UMyoDelegateBlueprint : public UMyoDelegate
{
	GENERATED_UINTERFACE_BODY()
};

class IMyoDelegateBlueprint : public IMyoDelegate
{
	GENERATED_IINTERFACE_BODY()

public:
	UObject* ValidSelfPointer;
	UObject* InterfaceDelegate;
	TArray<UMyoController*> LatestFrame;

	virtual UMyoController* MyoPrimaryMyo();
	virtual UMyoController* MyoLeftMyo();
	virtual UMyoController* MyoRightMyo();
	virtual void MyoSetLockingPolicy(MyoLockingPolicy policy);
	virtual bool MyoIsHubEnabled() override;
	virtual void MyoConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted) override;
	virtual void MyoStartup() override;
	virtual void MyoShutdown() override;
	virtual void MyoTick(float DeltaTime) override;
	virtual bool MyoIsValidId(int32 myoId) override;
	void SetInterfaceDelegate(UObject* newDelegate);
	bool IsValidDelegate();
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
	UMyoController* InternalAddController(int newId);
	UMyoController* InternalControllerForId(int32 myoId);
};
