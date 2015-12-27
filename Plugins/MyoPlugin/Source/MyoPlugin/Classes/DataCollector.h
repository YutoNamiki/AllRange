#pragma once

#include "MyoDelegate.h"
#include "HAL/ThreadingBase.h"
#include "DataCollector.generated.h"

struct InputInformation
{
	void* Ptr;
	float RotationX;
	float RotationY;
	float RotationZ;
	float RotationW;
	float AccelerationX;
	float AccelerationY;
	float AccelerationZ;
	float GyroX;
	float GyroY;
	float GyroZ;
	char Emg0;
	char Emg1;
	char Emg2;
	char Emg3;
	char Emg4;
	char Emg5;
	char Emg6;
	char Emg7;
	char Pose;
	char WhichArm;
	char ArmDirection;
	bool OnPair;
	bool OnConnect;
	bool OnArmSync;
	bool OnLock;
};

struct OutputInformation
{
	void* Ptr;
	char Vibration;
	char LockingPolicy;
	char UnlockType;
	char StreamEmgType;
	bool IsLockOrder;
};

class SendDataWorker : public FRunnable
{
public:

	SendDataWorker(FCriticalSection& mutex, TArray<OutputInformation>& sendData);
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FThreadSafeCounter stopTaskCounter;

	TArray<OutputInformation>* sendData;
	FCriticalSection* mutex;
};

class ReceiveDataWorker : public FRunnable
{
public:

	ReceiveDataWorker(FCriticalSection& mutex, InputInformation& receiveData);
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FThreadSafeCounter stopTaskCounter;
	InputInformation* receiveData;
	FCriticalSection* mutex;
};

UCLASS()
class MYOPLUGIN_API UDataCollector : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<uint64> KnownMyos;
	UPROPERTY()
	int32 LeftMyo;
	UPROPERTY()
	int32 RightMyo;
	UPROPERTY()
	uint64 LastPairedMyo;
	UPROPERTY()
	TArray<FMyoDeviceData> Data;
	UPROPERTY()
	bool Enabled;
	UPROPERTY()
	bool Listening;
	UPROPERTY()
	MyoLockingPolicy LockingPolicy = MyoLockingPolicy::Standard;
	UPROPERTY()
	MyoPose LastPose;
	UPROPERTY()
	FRotator ArmSpaceCorrection;
	UPROPERTY()
	bool CorrectionAvailable = false;

	IMyoDelegate* MyoDelegate;

	UDataCollector(class FObjectInitializer const& objectInitializer);
	~UDataCollector();

	void PressPose(MyoPose pose);
	void ReleasePose(MyoPose pose);
	void Tick(float deltaTime);
	void ConvertData(FCriticalSection& mutex, InputInformation& data, uint64& id, FQuat& rot, FVector& accel, FVector& gyro, TArray<int8>& emg,
		MyoPose& pose, MyoArm& arm, MyoArmDirection& direction, bool& pair, bool& connect, bool& armSync, bool& lock);
	void OnConnect(uint64 myoId);
	void OnDisconnect(uint64 myoId);
	void OnArmSync(uint64 myoId, MyoArm arm, MyoArmDirection xDirection);
	void OnArmUnsync(uint64 myoId);
	void OnPair(uint64 myoId);
	void OnUnpair(uint64 myoId);
	void OnOrientationData(uint64 myoId, FQuat& quat);
	void OnAccelerometerData(uint64 myoId, FVector& accel);
	void OnGyroscopeData(uint64 myoId, FVector& gyro);
	void OnUnlock(uint64 myoId);
	void OnLock(uint64 myoId);
	void OnPose(uint64 myoId, MyoPose pose);
	void OnEmgData(uint64 myoId, TArray<int8>& emg);
	int32 IdentifyMyo(uint64 myoId);
	uint64 LastValidMyo();
	bool MyoIsValidForInputMapping(uint64 myoId);
	int32 MyoIndexForMyo(uint64 myoId);
	void UnlockMyo(uint64 myoId, MyoUnlockType type = MyoUnlockType::Hold);
	void LockMyo(uint64 myoId);
	bool Startup();
	void ShutDown();
	void ResetHub();
	void SetLockingPolicy(MyoLockingPolicy policy);
	void SetStreamEmg(uint64 myoId, MyoStreamEmgType type);
	void VibrateDevice(uint64 myoId, MyoVibrationType type);
		
	static FRotator CombineRotators(FRotator a, FRotator b);

private:
	TArray<OutputInformation> sendData;
	InputInformation receiveData;
	FCriticalSection mutex;

	FRunnableThread* sendThread = nullptr;
	FRunnableThread* receiveThread = nullptr;
	SendDataWorker* sendDataWorker = nullptr;
	ReceiveDataWorker* receiveDataWorker = nullptr;
	int32 sendThreadCount = 0;
	int32 receiveThreadCount = 0;
};
