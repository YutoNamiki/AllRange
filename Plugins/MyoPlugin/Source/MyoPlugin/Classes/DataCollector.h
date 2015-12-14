#pragma once

#include "MyoDelegate.h"
#include "HAL/ThreadingBase.h"
#include "DataCollector.generated.h"

class FIPv4Address;
class FSocket;
class FInternetAddr;
class ISocketSubsystem;

class SendDataWorker : public FRunnable
{
public:

	SendDataWorker(FCriticalSection& mutex, FString myoDriverIP, uint32 port, TArray<uint8>& sendData);
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	FThreadSafeCounter stopTaskCounter;
	TSharedPtr<ISocketSubsystem> socketSubSystem;
	TSharedPtr<FSocket> socket;
	TSharedPtr<FIPv4Address> ipv4Address;
	TSharedPtr<FInternetAddr> address;
	FString myoDriverIP;
	uint32 port;
	TArray<uint8>* sendData;
	FCriticalSection* mutex;
};

class ReceiveDataWorker : public FRunnable
{
public:

	ReceiveDataWorker(FCriticalSection& mutex, FString myoDriverIP, uint32 port, TArray<uint8>& receiveData);
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	FThreadSafeCounter stopTaskCounter;
	TSharedPtr<ISocketSubsystem> socketSubSystem;
	TSharedPtr<FSocket> socket;
	TSharedPtr<FIPv4Address> ipv4Address;
	TSharedPtr<FInternetAddr> address;
	FString myoDriverIP;
	uint32 port;
	TArray<uint8>* receiveData;
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
	void OnConnect(uint64 myoId);
	void OnDisconnect(uint64 myoId);
	void OnArmSync(uint64 myoId, MyoArm arm, MyoArmDirection xDirection);
	void OnArmUnsync(uint64 myoId);
	void OnPair(uint64 myoId);
	void OnUnpair(uint64 myoId);
	void OnOrientationData(uint64 myoId, FRotator& rotation);
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
	void StartListening();
	void StopListening();
	void UnlockHoldEachMyo();
	void LockEachMyo();
	bool Startup();
	void ShutDown();
	void ResetHub();
	void SetLockingPolicy(MyoLockingPolicy policy);

private:
	FString myoDriverIP = "127.0.0.1";
	uint32 sendPort = 8000;
	uint32 receivePort = 8001;
	TArray<uint8> sendData;
	TArray<uint8> receiveData;
	FCriticalSection mutex;

	TSharedPtr<FRunnableThread> sendThread;
	TSharedPtr<FRunnableThread> receiveThread;
	TSharedPtr<SendDataWorker> sendDataWorker;
	TSharedPtr<ReceiveDataWorker> receiveDataWorker;
};
