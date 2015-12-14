#pragma once

#include "IMyoPlugin.h"

class UDataCollector;
struct FKey;

struct EMyoKeys
{
	static const FKey PoseRest;
	static const FKey PoseFist;
	static const FKey PoseWaveIn;
	static const FKey PoseWaveOut;
	static const FKey PoseFingersSpread;
	static const FKey PoseDoubleTap;
	static const FKey PoseUnknown;
	static const FKey AccelerationX;
	static const FKey AccelerationY;
	static const FKey AccelerationZ;
	static const FKey OrientationPitch;
	static const FKey OrientationYaw;
	static const FKey OrientationRoll;
	static const FKey GyroX;
	static const FKey GyroY;
	static const FKey GyroZ;
};

class FMyoPlugin : public IMyoPlugin
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void SetDelegate(IMyoDelegate* newDelegate) override;
	virtual void RemoveDelegate() override;
	virtual void MyoTick(float DeltaTime) override;
	virtual void VibrateDevice(int32 deviceId, MyoVibrationType vibrationType) override;
	virtual FMyoDeviceData* LatestData(int32 deviceId) override;
	virtual void WhichArm(int32 deviceId, MyoArm& arm) override;
	virtual void LeftMyoId(bool& available, int32& deviceId) override;
	virtual void RightMyoId(bool& available, int32& deviceId) override;
	virtual void PrimaryMyoId(bool& available, int32& deviceId) override;
	virtual void MaxMyoId(int32& maxId) override;
	virtual bool IsHubEnabled() override;
	virtual bool IsValidDeviceId(int32 deviceId) override;
	virtual void CalibrateOrientation(int32 deviceId, FRotator direction) override;
	virtual void SetLockingPolicy(MyoLockingPolicy policy) override;
	virtual void UnlockMyo(int32 deviceId, MyoUnlockType type) override;
	virtual void LockMyo(int32 deviceId) override;
	virtual void SetStreamEmg(int32 deviceId, MyoStreamEmgType type) override;

private:
	UDataCollector* collector;

	static FRotator CombineRotators(FRotator a, FRotator b);
};
