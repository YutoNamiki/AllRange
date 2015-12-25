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

SendDataWorker::SendDataWorker(FCriticalSection& mutex, TArray<uint8>& sendData)
{
	this->mutex = &mutex;
	this->sendData = &sendData;
	this->sendData->Init(-1, 256);
}

uint32 SendDataWorker::Run()
{
	const auto fileMapName = "MyoReceiveData";
	char* pString = nullptr;
	HANDLE handleFileMemory = nullptr;

	while (stopTaskCounter.GetValue() == 0)
	{
		handleFileMemory = CreateFileMappingA(reinterpret_cast<HANDLE>(-1), nullptr, PAGE_READWRITE, 0, static_cast<unsigned long>(sendData->Num() + 1), fileMapName);
		pString = static_cast<char*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pString == nullptr)
			continue;
		auto message = reinterpret_cast<char*>(sendData->GetData());
		auto size = sendData->Num();
		memcpy_s(pString, size, message, size);
		if (UnmapViewOfFile(pString) == 0)
			continue;
		pString = nullptr;
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

ReceiveDataWorker::ReceiveDataWorker(FCriticalSection& mutex, TArray<uint8>& receiveData)
{
	this->mutex = &mutex;
	this->receiveData = &receiveData;
	this->receiveData->Init(0, 58);
}

uint32 ReceiveDataWorker::Run()
{
	const auto fileMapName = "MyoSendData";
	char* pString = nullptr;
	HANDLE handleFileMemory = nullptr;

	while (stopTaskCounter.GetValue() == 0)
	{
		handleFileMemory = OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, fileMapName);
		pString = static_cast<char*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pString == nullptr)
			continue;
		{
			FScopeLock lock(mutex);
			for (auto i = 0; i < receiveData->Num(); i++)
				(*receiveData)[i] = pString[i];
		}
		if (UnmapViewOfFile(pString) == 0)
			continue;
		CloseHandle(handleFileMemory);
		handleFileMemory = nullptr;
		pString = nullptr;
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

}

UDataCollector::~UDataCollector()
{
	ShutDown();
}

void UDataCollector::PressPose(MyoPose pose)
{
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
}

void UDataCollector::ReleasePose(MyoPose pose)
{
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
}

void UDataCollector::Tick(float deltaTime)
{
	uint64 id = 0;
	FQuat rotation = FQuat::Identity;
	FVector acceleration = FVector::ZeroVector, gyro = FVector::ZeroVector;
	TArray<int8> emg;
	emg.Init(0, 8);
	MyoPose pose;
	MyoArm whichArm;
	MyoArmDirection xDirection;
	bool onPair = false, onConnect = false, onArmSync = false, onLock = true;

	ConvertData(mutex, receiveData.GetData(), id, rotation, acceleration, gyro, emg, pose, 
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
}

void UDataCollector::ConvertData(FCriticalSection& mutex, uint8* data, uint64& id, FQuat& rot, FVector& accel, FVector& gyro, 
	TArray<int8>& emg, MyoPose & pose, MyoArm & arm, MyoArmDirection & direction, bool& pair, bool& connect, bool& armSync, bool& lock)
{
	FScopeLock threadLock(&mutex);
	id = (static_cast<uint64>(data[0]) << 0) | (static_cast<uint64>(data[1]) << 8) | (static_cast<uint64>(data[2]) << 16) | (static_cast<uint64>(data[3]) << 24) |
		(static_cast<uint64>(data[4]) << 32) | (static_cast<uint64>(data[5]) << 40) | (static_cast<uint64>(data[6]) << 48) | (static_cast<uint64>(data[7]) << 56);
	uint32 rx = (static_cast<uint32>(data[8]) << 0) | (static_cast<uint32>(data[9]) << 8) | (static_cast<uint32>(data[10]) << 16) | (static_cast<uint32>(data[11]) << 24);
	uint32 ry = (static_cast<uint32>(data[12]) << 0) | (static_cast<uint32>(data[13]) << 8) | (static_cast<uint32>(data[14]) << 16) | (static_cast<uint32>(data[15]) << 24);
	uint32 rz = (static_cast<uint32>(data[16]) << 0) | (static_cast<uint32>(data[17]) << 8) | (static_cast<uint32>(data[18]) << 16) | (static_cast<uint32>(data[19]) << 24);
	uint32 rw = (static_cast<uint32>(data[20]) << 0) | (static_cast<uint32>(data[21]) << 8) | (static_cast<uint32>(data[22]) << 16) | (static_cast<uint32>(data[23]) << 24);
	rot = FQuat(*reinterpret_cast<float*>(&rx), *reinterpret_cast<float*>(&ry), *reinterpret_cast<float*>(&rz), *reinterpret_cast<float*>(&rw));
	uint32 ax = (static_cast<uint32>(data[24]) << 0) | (static_cast<uint32>(data[25]) << 8) | (static_cast<uint32>(data[26]) << 16) | (static_cast<uint32>(data[27]) << 24);
	uint32 ay = (static_cast<uint32>(data[28]) << 0) | (static_cast<uint32>(data[29]) << 8) | (static_cast<uint32>(data[30]) << 16) | (static_cast<uint32>(data[31]) << 24);
	uint32 az = (static_cast<uint32>(data[32]) << 0) | (static_cast<uint32>(data[33]) << 8) | (static_cast<uint32>(data[34]) << 16) | (static_cast<uint32>(data[35]) << 24);
	accel = FVector(*reinterpret_cast<float*>(&ax), *reinterpret_cast<float*>(&ay), *reinterpret_cast<float*>(&az));
	uint32 gx = (static_cast<uint32>(data[36]) << 0) | (static_cast<uint32>(data[37]) << 8) | (static_cast<uint32>(data[38]) << 16) | (static_cast<uint32>(data[39]) << 24);
	uint32 gy = (static_cast<uint32>(data[40]) << 0) | (static_cast<uint32>(data[41]) << 8) | (static_cast<uint32>(data[42]) << 16) | (static_cast<uint32>(data[43]) << 24);
	uint32 gz = (static_cast<uint32>(data[44]) << 0) | (static_cast<uint32>(data[45]) << 8) | (static_cast<uint32>(data[46]) << 16) | (static_cast<uint32>(data[47]) << 24);
	gyro = FVector(*reinterpret_cast<float*>(&gx), *reinterpret_cast<float*>(&gy), *reinterpret_cast<float*>(&gz));
	emg[0] = data[48];	emg[1] = data[49];	emg[2] = data[50];	emg[3] = data[51];
	emg[4] = data[52];	emg[5] = data[53];	emg[6] = data[54];	emg[7] = data[55];
	pose = static_cast<MyoPose>((data[56] & 0b11100000) >> 5);
	arm = static_cast<MyoArm>((data[56] & 0b00011100) >> 2);
	direction = static_cast<MyoArmDirection>((data[56] & 0b00000011) >> 0);
	pair = ((data[57] & 0b00001000) >> 3) == 1 ? true : false;
	connect = ((data[57] & 0b00000100) >> 2) == 1 ? true : false;
	armSync = ((data[57] & 0b00000010) >> 1) == 1 ? true : false;
	lock = ((data[57] & 0b00000001) >> 0) == 1 ? true : false;
}

void UDataCollector::OnConnect(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Log, TEXT("Myo %d  has connected."), IdentifyMyo(myoId));
	if (MyoDelegate)
		MyoDelegate->OnConnect(IdentifyMyo(myoId));
}

void UDataCollector::OnDisconnect(uint64 myoId)
{
	UE_LOG(MyoPluginLog, Log, TEXT("Myo %d  has disconnected."), IdentifyMyo(myoId));
	if (MyoDelegate)
		MyoDelegate->OnDisconnect(IdentifyMyo(myoId));
	if (myoId == LastPairedMyo)
		LastPairedMyo = LastValidMyo();
}

void UDataCollector::OnArmSync(uint64 myoId, MyoArm arm, MyoArmDirection xDirection)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Arm = arm;
	Data[myoIndex].XDirection = xDirection;
	if (arm == MyoArm::Left)
		LeftMyo = myoIndex + 1;
	if (arm == MyoArm::Right)
		RightMyo = myoIndex + 1;
	if (MyoDelegate)
		MyoDelegate->OnArmSync(IdentifyMyo(myoId), arm, xDirection);
}

void UDataCollector::OnArmUnsync(uint64 myoId)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Arm = MyoArm::Unknown;
	Data[myoIndex].XDirection = MyoArmDirection::Unknown;
	if (LeftMyo == myoIndex + 1)
		LeftMyo = -1;
	if (RightMyo == myoIndex + 1)
		RightMyo = -1;
	if (MyoDelegate)
		MyoDelegate->OnArmUnsync(IdentifyMyo(myoId));
}

void UDataCollector::OnPair(uint64 myoId)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	if (myoIndex == -1) 
	{
		KnownMyos.Add(myoId);
		FMyoDeviceData data;
		data.XDirection = MyoArmDirection::Unknown;
		data.Arm = MyoArm::Unknown;
		if (CorrectionAvailable)
			data.ArmSpaceCorrection = ArmSpaceCorrection;
		Data.Add(data);
	}
	if (MyoDelegate != nullptr)
		MyoDelegate->OnPair(IdentifyMyo(myoId));
	LastPairedMyo = myoId;
}

void UDataCollector::OnUnpair(uint64 myoId)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	if (MyoDelegate != nullptr)
		MyoDelegate->OnUnpair(IdentifyMyo(myoId));
}

void UDataCollector::OnOrientationData(uint64 myoId, FQuat& quat)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Quaternion.X = quat.X;
	Data[myoIndex].Quaternion.Y = quat.Y;
	Data[myoIndex].Quaternion.Z = quat.Z;
	Data[myoIndex].Quaternion.W = quat.W;
	Data[myoIndex].Orientation = ConvertOrientationToUE(FRotator(Data[myoIndex].Quaternion));
	Data[myoIndex].ArmOrientation = ConvertOrientationToArmSpace(Data[myoIndex].Orientation, Data[myoIndex].ArmSpaceCorrection, Data[myoIndex].XDirection);
	if (MyoDelegate)
	{
		MyoDelegate->OnOrientationData(myoIndex + 1, Data[myoIndex].Quaternion);
		MyoDelegate->OnOrientationData(myoIndex + 1, Data[myoIndex].ArmOrientation);
		if (MyoIsValidForInputMapping(myoId))
		{
			EmitAnalogInputEventForKey(EMyoKeys::OrientationPitch, Data[myoIndex].ArmOrientation.Pitch * OrientationScale.Pitch, 0);
			EmitAnalogInputEventForKey(EMyoKeys::OrientationYaw, Data[myoIndex].ArmOrientation.Yaw * OrientationScale.Yaw, 0);
			EmitAnalogInputEventForKey(EMyoKeys::OrientationRoll, Data[myoIndex].ArmOrientation.Roll * OrientationScale.Roll, 0);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Orientation: { Pitch: %f, Yaw: %f, Roll: %f } ArmOrientation { Pitch: %f, Yaw: %f, Roll: %f }"),
	//	Data[myoIndex].Orientation.Pitch, Data[myoIndex].Orientation.Yaw, Data[myoIndex].Orientation.Roll,
	//	Data[myoIndex].ArmOrientation.Pitch, Data[myoIndex].ArmOrientation.Yaw, Data[myoIndex].ArmOrientation.Roll);
}

void UDataCollector::OnAccelerometerData(uint64 myoId, FVector& accel)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Acceleration.X = accel.X;
	Data[myoIndex].Acceleration.Y = accel.Y;
	Data[myoIndex].Acceleration.Z = accel.Z;
	Data[myoIndex].Acceleration = ConvertVectorToUE(Data[myoIndex].Acceleration);
	Data[myoIndex].ArmAcceleration = Data[myoIndex].Acceleration;
	Data[myoIndex].BodySpaceNullAcceleration = ConvertAccelerationToBodySpace(Data[myoIndex].ArmAcceleration, Data[myoIndex].Orientation, Data[myoIndex].ArmSpaceCorrection, Data[myoIndex].XDirection);
	if (MyoDelegate)
	{
		MyoDelegate->OnAccelerometerData(myoIndex + 1, Data[myoIndex].Acceleration);
		MyoDelegate->OnArmMoved(myoIndex + 1, Data[myoIndex].ArmAcceleration, Data[myoIndex].ArmOrientation, Data[myoIndex].ArmGyro, Data[myoIndex].Pose);																																		//InputMapping - only supports controller 1 for now
		if (MyoIsValidForInputMapping(myoId))
		{
			EmitAnalogInputEventForKey(EMyoKeys::AccelerationX, Data[myoIndex].ArmAcceleration.X, 0);
			EmitAnalogInputEventForKey(EMyoKeys::AccelerationY, Data[myoIndex].ArmAcceleration.Y, 0);
			EmitAnalogInputEventForKey(EMyoKeys::AccelerationZ, Data[myoIndex].ArmAcceleration.Z, 0);
		}
	}
}

void UDataCollector::OnGyroscopeData(uint64 myoId, FVector& gyro)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Gyro.X = gyro.X;
	Data[myoIndex].Gyro.Y = gyro.Y;
	Data[myoIndex].Gyro.Z = gyro.Z;
	Data[myoIndex].ArmGyro = ConvertVectorToUE(Data[myoIndex].Gyro);
	if (MyoDelegate)
	{
		MyoDelegate->OnGyroscopeData(myoIndex + 1, Data[myoIndex].Gyro);
		if (MyoIsValidForInputMapping(myoId))
		{
			EmitAnalogInputEventForKey(EMyoKeys::GyroX, Data[myoIndex].ArmGyro.X * GyroScale, 0);
			EmitAnalogInputEventForKey(EMyoKeys::GyroY, Data[myoIndex].ArmGyro.Y * GyroScale, 0);
			EmitAnalogInputEventForKey(EMyoKeys::GyroZ, Data[myoIndex].ArmGyro.Z * GyroScale, 0);
		}
	}
}

void UDataCollector::OnUnlock(uint64 myoId)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].IsLocked = false;
}

void UDataCollector::OnLock(uint64 myoId)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].IsLocked = true;
}

void UDataCollector::OnPose(uint64 myoId, MyoPose pose)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	Data[myoIndex].Pose = pose;
	UE_LOG(MyoPluginLog, Log, TEXT("Myo %d switched to pose %s."), IdentifyMyo(myoId), *ConvertPoseToString(pose));
	if (MyoDelegate)
	{
		MyoDelegate->OnPose(myoIndex + 1, pose);
		if (MyoIsValidForInputMapping(myoId))
		{
			ReleasePose(LastPose);
			PressPose(pose);
			LastPose = pose;
		}
	}
}

void UDataCollector::OnEmgData(uint64 myoId, TArray<int8>& emg)
{
	auto myoIndex = MyoIndexForMyo(myoId);
	FMyoEmgData data;
	for (auto emgValue : emg)
		data.Streams.Add(emgValue);
	MyoDelegate->OnEmgData(myoIndex + 1, data);
}

int32 UDataCollector::IdentifyMyo(uint64 myoId)
{
	for (auto i = 0; i < KnownMyos.Num(); ++i) 
	{
		if (KnownMyos[i] == myoId)
			return i + 1;
	}
	return 0;
}

uint64 UDataCollector::LastValidMyo()
{
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
	return (myoId == LastPairedMyo);
}

int32 UDataCollector::MyoIndexForMyo(uint64 myoId)
{
	return IdentifyMyo(myoId) - 1;
}

void UDataCollector::UnlockHoldEachMyo()
{
	
}

void UDataCollector::LockEachMyo()
{

}

bool UDataCollector::Startup()
{
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

	return true;
}

void UDataCollector::ShutDown()
{
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
}

void UDataCollector::ResetHub()
{
	ShutDown();
	Startup();
}

void UDataCollector::SetLockingPolicy(MyoLockingPolicy policy)
{

}

FRotator UDataCollector::CombineRotators(FRotator a, FRotator b)
{
	return FRotator(FQuat(b) * FQuat(a));
}

bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent KeyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

bool EmitAnalogInputEventForKey(FKey key, float value, int user)
{
	FAnalogInputEvent AnalogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, false, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(AnalogInputEvent);
}

FRotator ConvertOrientationToUE(FRotator rawOrientation)
{
	return FRotator(rawOrientation.Pitch * -1.0f, rawOrientation.Yaw * -1.0f, rawOrientation.Roll);
}

FVector ConvertVectorToUE(FVector rawAcceleration)
{
	return FVector(rawAcceleration.X, -rawAcceleration.Y, rawAcceleration.Z);
}

FVector ConvertAccelerationToBodySpace(FVector armAcceleration, FRotator orientation, FRotator armCorrection, MyoArmDirection direction)
{
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
