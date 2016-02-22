#include "MyoPluginPrivatePCH.h"
#include "DataCollector.h"
#include "FMyoPlugin.h"

#include "AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "HideWindowsPlatformTypes.h"

static const FRotator OrientationScale = FRotator(1.0f / 90.0f, 1.0f / 180.0f, 1.0f / 180.0f);
static const float GyroScale = 1.0f / 45.0f;

bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat);
bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat);
bool EmitAnalogInputEventForKey(FKey key, float value, int user); 
FRotator ConvertOrientationToUE(FRotator rawOrientation);
FVector ConvertVectorToUE(FVector rawAcceleration);
FVector ConvertAccelerationToBodySpace(FVector armAcceleration, FRotator orientation, FRotator armCorrection, MyoArmDirection direction);
FRotator ConvertOrientationToArmSpace(FRotator convertedOrientation, FRotator armCorrection, MyoArmDirection direction);
FString ConvertPoseToString(MyoPose pose);

SendDataWorker::SendDataWorker(FCriticalSection& mutex, TArray<OutputInformation>& sendData)
{
	this->mutex = &mutex;
	this->sendData = &sendData;
}

uint32 SendDataWorker::Run()
{
	const auto fileMapName = "MyoReceiveData";
	auto size = sizeof(OutputInformation);
	OutputInformation* pData = nullptr;
	HANDLE handleFileMemory = nullptr;

	while (stopTaskCounter.GetValue() == 0)
	{
		if (sendData->Num() <= 0)
			continue;
		pData = static_cast<OutputInformation*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pData == nullptr)
			continue;
		{
			FScopeLock lock(mutex);
			memcpy_s(pData, size, &(*sendData)[0], size);
			sendData->RemoveAt(0);
		}
		if (UnmapViewOfFile(pData) == 0)
			continue;
		pData = nullptr;
		FPlatformProcess::Sleep(1.0f / 90.0f);
	}
	CloseHandle(handleFileMemory);
	handleFileMemory = nullptr;
	
	return 0;
}

void SendDataWorker::Stop()
{
	
	stopTaskCounter.Increment();
}

ReceiveDataWorker::ReceiveDataWorker(FCriticalSection& mutex, InputInformation& receiveData)
{
	this->mutex = &mutex;
	this->receiveData = &receiveData;
}

uint32 ReceiveDataWorker::Run()
{
	const auto fileMapName = "MyoSendData";
	auto size = sizeof(InputInformation);
	InputInformation* pData = nullptr;
	HANDLE handleFileMemory = nullptr;

	while (stopTaskCounter.GetValue() == 0)
	{
		handleFileMemory = OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, fileMapName);
		pData = static_cast<InputInformation*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pData == nullptr)
			continue;
		{
			FScopeLock lock(mutex);
			memcpy_s(receiveData, size, pData, size);
		}
		if (UnmapViewOfFile(pData) == 0)
			continue;
		CloseHandle(handleFileMemory);
		handleFileMemory = nullptr;
		pData = nullptr;
		FPlatformProcess::Sleep(1.0f / 90.0f);
	}
	if (handleFileMemory != nullptr)
	{
		CloseHandle(handleFileMemory);
		handleFileMemory = nullptr;
	}
	return 0;
}

void ReceiveDataWorker::Stop()
{
	stopTaskCounter.Increment();
}

UDataCollector::UDataCollector(class FObjectInitializer const& objectInitializer)
	: Super(objectInitializer)
	, MyoDelegate(nullptr)
	, Enabled(false)
	, Listening(false)
	, LastPose(MyoPose::Unknown)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::UDataCollector"));
}

UDataCollector::~UDataCollector()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::~UDataCollector_Begin"));
	ShutDown();
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::~UDataCollector_End"));
}

void UDataCollector::PressPose(MyoPose pose)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::PressPose_Begin"));
	switch (pose)
	{
	case MyoPose::Rest:
		EmitKeyDownEventForKey(EMyoKeys::PoseRest, 0, 0);
		break;
	case MyoPose::Fist:
		EmitKeyDownEventForKey(EMyoKeys::PoseFist, 0, 0);
		break;
	case MyoPose::WaveIn:
		EmitKeyDownEventForKey(EMyoKeys::PoseWaveIn, 0, 0);
		break;
	case MyoPose::WaveOut:
		EmitKeyDownEventForKey(EMyoKeys::PoseWaveOut, 0, 0);
		break;
	case MyoPose::FingerSpread:
		EmitKeyDownEventForKey(EMyoKeys::PoseFingersSpread, 0, 0);
		break;
	case MyoPose::DoubleTap:
		EmitKeyDownEventForKey(EMyoKeys::PoseDoubleTap, 0, 0);
		break;
	case MyoPose::Unknown:
		EmitKeyDownEventForKey(EMyoKeys::PoseUnknown, 0, 0);
		break;
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::PressPose_End"));
}

void UDataCollector::ReleasePose(MyoPose pose)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ReleasePose_Begin"));
	switch (pose)
	{
	case MyoPose::Rest:
		EmitKeyUpEventForKey(EMyoKeys::PoseRest, 0, 0);
		break;
	case MyoPose::Fist:
		EmitKeyUpEventForKey(EMyoKeys::PoseFist, 0, 0);
		break;
	case MyoPose::WaveIn:
		EmitKeyUpEventForKey(EMyoKeys::PoseWaveIn, 0, 0);
		break;
	case MyoPose::WaveOut:
		EmitKeyUpEventForKey(EMyoKeys::PoseWaveOut, 0, 0);
		break;
	case MyoPose::FingerSpread:
		EmitKeyUpEventForKey(EMyoKeys::PoseFingersSpread, 0, 0);
		break;
	case MyoPose::DoubleTap:
		EmitKeyUpEventForKey(EMyoKeys::PoseDoubleTap, 0, 0);
		break;
	case MyoPose::Unknown:
		EmitKeyUpEventForKey(EMyoKeys::PoseUnknown, 0, 0);
		break;
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ReleasePose_End"));
}

void UDataCollector::Tick(float deltaTime)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::Tick_Begin"));

	uint64 id = 0;
	FQuat rotation = FQuat::Identity;
	FVector acceleration = FVector::ZeroVector, gyro = FVector::ZeroVector;
	TArray<int8> emg;
	emg.Init(0, 8);
	MyoPose pose;
	MyoArm whichArm;
	MyoArmDirection xDirection;
	bool onPair = false, onConnect = false, onArmSync = false, onLock = true;

	ConvertData(mutex, receiveData, id, rotation, acceleration, gyro, emg, pose, 
		whichArm, xDirection, onPair, onConnect, onArmSync, onLock);
	auto index = MyoIndexForMyo(id);
	if (index == -1 && onConnect)
		OnConnect(id);
	else if (index != -1 && !onConnect)
		OnDisconnect(id);
	if (index == -1 && onPair)
		OnPair(id);
	else if (index != -1 && !onPair)
		OnUnpair(id);
	if (index == -1 && onArmSync)
		OnArmSync(id, whichArm, xDirection);
	else if (index != -1 && !onArmSync)
		OnArmUnsync(id);
	if (index == -1)
		onLock ? OnLock(id) : OnUnlock(id);
	else if (index != -1)
	{
		if (Data[index].IsLocked != onLock)
			onLock ? OnLock(id) : OnUnlock(id);
	}
	if (onConnect && onArmSync && onPair)
	{
		OnOrientationData(id, rotation);
		OnAccelerometerData(id, acceleration);
		OnGyroscopeData(id, gyro);
		OnEmgData(id, emg);
		if (index == -1)
			OnPose(id, pose);
		else if (index != 1)
		{
			if (Data[index].Pose != pose)
				OnPose(id, pose);
		}
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::Tick_End"));
}

void UDataCollector::ConvertData(FCriticalSection& mutex, InputInformation& data, uint64& id, FQuat& rot, FVector& accel, FVector& gyro,
	TArray<int8>& emg, MyoPose & pose, MyoArm & arm, MyoArmDirection & direction, bool& pair, bool& connect, bool& armSync, bool& lock)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ConvertData_Begin"));
	FScopeLock threadLock(&mutex);
	id = reinterpret_cast<uint64>(data.Ptr);
	rot = FQuat(data.RotationX, data.RotationY, data.RotationZ, data.RotationW);
	accel = FVector(data.AccelerationX, data.AccelerationY, data.AccelerationZ);
	gyro = FVector(data.GyroX, data.GyroY, data.GyroZ);
	emg[0] = data.Emg0;
	emg[1] = data.Emg1;
	emg[2] = data.Emg2;
	emg[3] = data.Emg3;
	emg[4] = data.Emg4;
	emg[5] = data.Emg5;
	emg[6] = data.Emg6;
	emg[7] = data.Emg7;
	pose = (data.Pose == 255) ? MyoPose::Unknown : static_cast<MyoPose>(data.Pose);
	arm = static_cast<MyoArm>(data.WhichArm);
	direction = static_cast<MyoArmDirection>(data.ArmDirection);
	pair = data.OnPair;
	connect = data.OnConnect;
	armSync = data.OnArmSync;
	lock = data.OnLock;
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ConvertData_End"));
}

void UDataCollector::OnConnect(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnConnect_Begin"));
	UE_LOG(MyoPluginLog, Log, TEXT("Myo %d  has connected."), IdentifyMyo(myoId));
	if (MyoDelegate)
		MyoDelegate->OnConnectFunction(IdentifyMyo(myoId));
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnConnect_End"));
}

void UDataCollector::OnDisconnect(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnDisconnect_Begin"));
	UE_LOG(MyoPluginLog, Log, TEXT("Myo %d  has disconnected."), IdentifyMyo(myoId));
	if (MyoDelegate)
		MyoDelegate->OnDisconnectFunction(IdentifyMyo(myoId));
	if (myoId == LastPairedMyo)
		LastPairedMyo = LastValidMyo();
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnDisconnect_End"));
}

void UDataCollector::OnArmSync(uint64 myoId, MyoArm arm, MyoArmDirection xDirection)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnArmSync_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Arm = arm;
	Data[myoIndex].XDirection = xDirection;
	if (arm == MyoArm::Left)
		LeftMyo = myoIndex + 1;
	if (arm == MyoArm::Right)
		RightMyo = myoIndex + 1;
	if (MyoDelegate)
		MyoDelegate->OnArmSyncFunction(IdentifyMyo(myoId), arm, xDirection);
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnArmSync_End"));
}

void UDataCollector::OnArmUnsync(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnArmUnsync_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	UE_LOG(MyoPluginLog, Warning, TEXT("myoIndex:%d"), myoIndex);
	Data[myoIndex].Arm = MyoArm::Unknown;
	Data[myoIndex].XDirection = MyoArmDirection::Unknown;
	if (LeftMyo == myoIndex + 1)
		LeftMyo = -1;
	if (RightMyo == myoIndex + 1)
		RightMyo = -1;
	if (MyoDelegate)
		MyoDelegate->OnArmUnsyncFunction(IdentifyMyo(myoId));
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnArmUnsync_End"));
}

void UDataCollector::OnPair(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnPair_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	if (myoIndex == -1) 
	{
		KnownMyos.Add(myoId);
		FMyoDeviceData data;
		data.XDirection = MyoArmDirection::Unknown;
		data.Arm = MyoArm::Unknown;
		data.ArmSpaceCorrection = ArmSpaceCorrection;
		Data.Add(data);
	}
	if (MyoDelegate != nullptr)
		MyoDelegate->OnPairFunction(IdentifyMyo(myoId));
	LastPairedMyo = myoId;
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnPair_End"));
}

void UDataCollector::OnUnpair(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnUnpair_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	if (MyoDelegate != nullptr)
		MyoDelegate->OnUnpairFunction(IdentifyMyo(myoId));
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnUnpair_End"));
}

void UDataCollector::OnOrientationData(uint64 myoId, FQuat& quat)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnOrientationData_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Quaternion.X = quat.X;
	Data[myoIndex].Quaternion.Y = quat.Y;
	Data[myoIndex].Quaternion.Z = quat.Z;
	Data[myoIndex].Quaternion.W = quat.W;
	Data[myoIndex].Orientation = ConvertOrientationToUE(FRotator(Data[myoIndex].Quaternion));
	Data[myoIndex].ArmOrientation = ConvertOrientationToArmSpace(Data[myoIndex].Orientation, Data[myoIndex].ArmSpaceCorrection, Data[myoIndex].XDirection);
	if (MyoDelegate)
	{
		MyoDelegate->OnOrientationDataFunction(myoIndex + 1, Data[myoIndex].Quaternion);
		MyoDelegate->OnOrientationDataFunction(myoIndex + 1, Data[myoIndex].ArmOrientation);
		if (MyoIsValidForInputMapping(myoId))
		{
			EmitAnalogInputEventForKey(EMyoKeys::OrientationPitch, Data[myoIndex].ArmOrientation.Pitch * OrientationScale.Pitch, 0);
			EmitAnalogInputEventForKey(EMyoKeys::OrientationYaw, Data[myoIndex].ArmOrientation.Yaw * OrientationScale.Yaw, 0);
			EmitAnalogInputEventForKey(EMyoKeys::OrientationRoll, Data[myoIndex].ArmOrientation.Roll * OrientationScale.Roll, 0);
		}
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnOrientationData_End"));
}

void UDataCollector::OnAccelerometerData(uint64 myoId, FVector& accel)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnAccelerometerData_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Acceleration.X = accel.X;
	Data[myoIndex].Acceleration.Y = accel.Y;
	Data[myoIndex].Acceleration.Z = accel.Z;
	Data[myoIndex].Acceleration = ConvertVectorToUE(Data[myoIndex].Acceleration);
	Data[myoIndex].ArmAcceleration = Data[myoIndex].Acceleration;
	Data[myoIndex].BodySpaceNullAcceleration = ConvertAccelerationToBodySpace(Data[myoIndex].ArmAcceleration, Data[myoIndex].Orientation, Data[myoIndex].ArmSpaceCorrection, Data[myoIndex].XDirection);
	if (MyoDelegate)
	{
		MyoDelegate->OnAccelerometerDataFunction(myoIndex + 1, Data[myoIndex].Acceleration);
		MyoDelegate->OnArmMovedFunction(myoIndex + 1, Data[myoIndex].ArmAcceleration, Data[myoIndex].ArmOrientation, Data[myoIndex].ArmGyro, Data[myoIndex].Pose);																																		//InputMapping - only supports controller 1 for now
		if (MyoIsValidForInputMapping(myoId))
		{
			EmitAnalogInputEventForKey(EMyoKeys::AccelerationX, Data[myoIndex].ArmAcceleration.X, 0);
			EmitAnalogInputEventForKey(EMyoKeys::AccelerationY, Data[myoIndex].ArmAcceleration.Y, 0);
			EmitAnalogInputEventForKey(EMyoKeys::AccelerationZ, Data[myoIndex].ArmAcceleration.Z, 0);
		}
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnAccelerometerData_End"));
}

void UDataCollector::OnGyroscopeData(uint64 myoId, FVector& gyro)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnGyroscopeData_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Gyro.X = gyro.X;
	Data[myoIndex].Gyro.Y = gyro.Y;
	Data[myoIndex].Gyro.Z = gyro.Z;
	Data[myoIndex].ArmGyro = ConvertVectorToUE(Data[myoIndex].Gyro);
	if (MyoDelegate)
	{
		MyoDelegate->OnGyroscopeDataFunction(myoIndex + 1, Data[myoIndex].Gyro);
		if (MyoIsValidForInputMapping(myoId))
		{
			EmitAnalogInputEventForKey(EMyoKeys::GyroX, Data[myoIndex].ArmGyro.X * GyroScale, 0);
			EmitAnalogInputEventForKey(EMyoKeys::GyroY, Data[myoIndex].ArmGyro.Y * GyroScale, 0);
			EmitAnalogInputEventForKey(EMyoKeys::GyroZ, Data[myoIndex].ArmGyro.Z * GyroScale, 0);
		}
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnGyroscopeData_End"));
}

void UDataCollector::OnUnlock(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnUnlock_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].IsLocked = false;
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnUnlock_End"));
}

void UDataCollector::OnLock(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnLock_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].IsLocked = true;
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnLock_End"));
}

void UDataCollector::OnPose(uint64 myoId, MyoPose pose)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnPose_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Pose = pose;
	UE_LOG(MyoPluginLog, Log, TEXT("Myo %d switched to pose %s."), IdentifyMyo(myoId), *ConvertPoseToString(pose));
	if (MyoDelegate)
	{
		MyoDelegate->OnPoseFunction(myoIndex + 1, pose);
		if (MyoIsValidForInputMapping(myoId))
		{
			ReleasePose(LastPose);
			PressPose(pose);
			LastPose = pose;
		}
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnPose_End"));
}

void UDataCollector::OnEmgData(uint64 myoId, TArray<int8>& emg)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnEmgData_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	FMyoEmgData data;
	for (auto emgValue : emg)
		data.Streams.Add(emgValue);
	if (MyoDelegate)
		MyoDelegate->OnEmgDataFunction(myoIndex + 1, data);
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::OnEmgData_End"));
}

int32 UDataCollector::IdentifyMyo(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::IdentifyMyo_Begin"));
	for (auto i = 0; i < KnownMyos.Num(); ++i) 
	{
		UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::IdentifyMyo_Loop"));
		if (KnownMyos[i] == myoId)
		{
			UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::IdentifyMyo_Break"));
			return i + 1;
		}
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::IdentifyMyo_End"));
	return 0;
}

uint64 UDataCollector::LastValidMyo()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::LastValidMyo"));
	for (auto i = 0; i < KnownMyos.Num(); ++i) 
	{
		auto myoId = KnownMyos[i];
		if (myoId != NULL)
			return KnownMyos[i];
	}
	return NULL;
}

bool UDataCollector::MyoIsValidForInputMapping(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::MyoIsValidForInputMapping"));
	return (myoId == LastPairedMyo);
}

int32 UDataCollector::MyoIndexForMyo(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::MyoIndexForMyo"));
	return IdentifyMyo(myoId) - 1;
}

void UDataCollector::UnlockMyo(uint64 myoId, MyoUnlockType type)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::UnlockMyo_Begin"));
	sendData.Add(OutputInformation());
	auto& data = sendData.Last();
	data.Ptr = reinterpret_cast<void*>(myoId);
	data.UnlockType = (type == MyoUnlockType::Timed) ? false : true;
	data.IsLockOrder = false;
	data.LockingPolicy = -1;
	data.StreamEmgType = -1;
	data.Vibration = -1;
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::UnlockMyo_End"));
}

void UDataCollector::LockMyo(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::LockMyo_Begin"));
	auto myoIndex = MyoIndexForMyo(myoId);
	sendData.Add(OutputInformation());
	auto& data = sendData.Last();
	data.Ptr = reinterpret_cast<void*>(myoId);
	data.IsLockOrder = true;
	data.LockingPolicy = -1;
	data.StreamEmgType = -1;
	data.UnlockType = -1;
	data.Vibration = -1;
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::LockMyo_End"));
}

bool UDataCollector::Startup()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::Startup_Begin"));
	if (sendDataWorker == nullptr)
		sendDataWorker = new SendDataWorker(mutex, sendData);
	if (receiveDataWorker == nullptr)
		receiveDataWorker = new ReceiveDataWorker(mutex, receiveData);
	if (sendThread == nullptr)
	{
		auto name = FString("SendDataThread_") + FString::FromInt(sendThreadCount++);
		sendThread = FRunnableThread::Create(sendDataWorker, *name);
	}
	if (receiveThread == nullptr)
	{
		auto name = FString("ReceiveDataThread_") + FString::FromInt(receiveThreadCount++);
		receiveThread = FRunnableThread::Create(receiveDataWorker, *name);
	}
	Enabled = true;
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::Startup_End"));
	return true;
}

void UDataCollector::ShutDown()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ShutDown_Begin"));
	if (sendThread != nullptr)
	{
		sendDataWorker->Stop();
		sendThread->WaitForCompletion();
		delete sendThread;
		sendThread = nullptr;
	}
	if (receiveThread != nullptr)
	{
		receiveDataWorker->Stop();
		receiveThread->WaitForCompletion();
		delete receiveThread;
		receiveThread = nullptr;
	}	
	if (sendDataWorker != nullptr)
	{
		delete sendDataWorker;
		sendDataWorker = nullptr;
	}
	if (receiveDataWorker != nullptr)
	{
		delete receiveDataWorker;
		receiveDataWorker = nullptr;
	}
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ShutDown_End"));
}

void UDataCollector::ResetHub()
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ResetHub_Begin"));
	ShutDown();
	Startup();
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::ResetHub_End"));
}

void UDataCollector::SetLockingPolicy(MyoLockingPolicy policy)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::SetLockingPolicy"));
	LockingPolicy = policy;
	sendData.Add(OutputInformation());
	auto& data = sendData.Last();
	data.Ptr = nullptr;
	data.IsLockOrder = false;
	data.StreamEmgType = -1;
	data.UnlockType = -1;
	data.LockingPolicy = (policy == MyoLockingPolicy::None) ? 0 : 1;
	data.Vibration = -1;
}

void UDataCollector::SetStreamEmg(uint64 myoId, MyoStreamEmgType type)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::SetStreamEmg"));
	sendData.Add(OutputInformation());
	auto& data = sendData.Last();
	data.Ptr = reinterpret_cast<void*>(myoId);
	data.StreamEmgType = static_cast<char>(type);
	data.IsLockOrder = false;
	data.LockingPolicy = -1;
	data.UnlockType = -1;
	data.Vibration = -1;
}

void UDataCollector::VibrateDevice(uint64 myoId, MyoVibrationType type)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::VibrateDevice"));
	sendData.Add(OutputInformation());
	auto& data = sendData.Last();
	data.Ptr = reinterpret_cast<void*>(myoId);
	data.Vibration = static_cast<char>(type);
	data.IsLockOrder = false;
	data.LockingPolicy = -1;
	data.StreamEmgType = -1;
	data.UnlockType = -1;
}

FRotator UDataCollector::CombineRotators(FRotator a, FRotator b)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("UDataCollector::CombineRotators"));
	return FRotator(FQuat(b) * FQuat(a));
}

bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("EmitKeyUpEventForKey"));
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("EmitKeyDownEventForKey"));
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int user)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("EmitAnalogInputEventForKey"));
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, false, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}

FRotator ConvertOrientationToUE(FRotator rawOrientation)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("ConvertOrientationToUE"));
	return FRotator(rawOrientation.Pitch * -1.0f, rawOrientation.Yaw * -1.0f, rawOrientation.Roll);
}

FVector ConvertVectorToUE(FVector rawAcceleration)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("ConvertVectorToUE"));
	return FVector(rawAcceleration.X, -rawAcceleration.Y, rawAcceleration.Z);
}

FVector ConvertAccelerationToBodySpace(FVector armAcceleration, FRotator orientation, FRotator armCorrection, MyoArmDirection direction)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("ConvertAccelerationToBodySpace"));
	auto directionModifier = 1.0f;
	if (direction == MyoArmDirection::Elbow)
		directionModifier = -1.0f;
	auto armYawCorrection = FRotator(0.0f, armCorrection.Yaw, 0.0f);
	auto fullCompensationOrientation = UDataCollector::CombineRotators(orientation, armYawCorrection);
	auto reactionAcceleration = fullCompensationOrientation.RotateVector(armAcceleration);
	return ((reactionAcceleration * FVector(directionModifier, directionModifier, 1.0f)) + FVector(0.0f, 0.0f, 1.0f)) * -1.0f;
}

FRotator ConvertOrientationToArmSpace(FRotator convertedOrientation, FRotator armCorrection, MyoArmDirection direction)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("ConvertOrientationToArmSpace"));
	auto directionModifier = 1.0f;
	if (direction == MyoArmDirection::Elbow)
	{
		directionModifier = -1.0f;
		convertedOrientation = FRotator(convertedOrientation.Pitch * directionModifier, convertedOrientation.Yaw, convertedOrientation.Roll * directionModifier);
	}
	auto tempRot = UDataCollector::CombineRotators(FRotator(0.0f, 0.0f, armCorrection.Roll * directionModifier), convertedOrientation);
	return UDataCollector::CombineRotators(tempRot, FRotator(0.0f, armCorrection.Yaw, 0.0f));
}

FString ConvertPoseToString(MyoPose pose)
{
	UE_LOG(MyoPluginLog, Warning, TEXT("ConvertPoseToString"));
	switch (pose)
	{
	case MyoPose::Rest:
		return FString("Rest");
	case MyoPose::Fist:
		return FString("Fist");
	case MyoPose::WaveIn:
		return FString("WaveIn");
	case MyoPose::WaveOut:
		return FString("WaveOut");
	case MyoPose::FingerSpread:
		return FString("FingerSpread");
	case MyoPose::DoubleTap:
		return FString("DoubleTap");
	case MyoPose::Unknown:
		return FString("Unknown");
	}
	return FString("");
}
