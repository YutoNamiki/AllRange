#include "MyoPluginPrivatePCH.h"
#include "DataCollector.h"
#include "FMyoPlugin.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

bool EmitKeyUpEventForKey(FKey key, int32 user, bool repeat);
bool EmitKeyDownEventForKey(FKey key, int32 user, bool repeat);
bool EmitAnalogInputEventForKey(FKey key, float value, int user);
FRotator ConvertOrientationToUE(FRotator rawOrientation);
FRotator CombineRotators(FRotator a, FRotator b);
FVector ConvertVectorToUE(FVector rawAcceleration);
FVector ConvertAccelerationToBodySpace(FVector armAcceleration, FRotator orientation, FRotator armCorrection, MyoArmDirection direction);
FRotator ConvertOrientationToArmSpace(FRotator convertedOrientation, FRotator armCorrection, MyoArmDirection direction);

SendDataWorker::SendDataWorker(FCriticalSection& mutex, FString myoDriverIP, uint32 port, TArray<uint8>& sendData)
{
	this->mutex = &mutex;
	this->myoDriverIP = myoDriverIP;
	this->port = port;
	this->sendData = &sendData;
}

bool SendDataWorker::Init()
{
	ipv4Address = TSharedPtr<FIPv4Address>(new FIPv4Address());
	FIPv4Address::Parse(myoDriverIP, *ipv4Address);
	socketSubSystem = TSharedPtr<ISocketSubsystem>(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM));
	address = socketSubSystem->CreateInternetAddr(ipv4Address->GetValue(), port);
	socket = TSharedPtr<FSocket>(socketSubSystem->CreateSocket(NAME_DGram, TEXT("MyoSendSocket"), true));
	socket->SetNonBlocking(true);
	socket->SetReuseAddr(true);

	return true;
}

uint32 SendDataWorker::Run()
{
	FString message = "Hello, Myo!";
	TCHAR* charMessage = message.GetCharArray().GetData();
	int32 sendSize = FCString::Strlen(charMessage);
	int32 sendSent = 0;
	auto serializedMessage = reinterpret_cast<uint8*>(TCHAR_TO_UTF8(charMessage));
	while (stopTaskCounter.GetValue() == 0)
	{
		if (socket->SendTo(serializedMessage, sendSize, sendSent, *address))
		{
			auto logString = FString("SendMessage: ") + FString(UTF8_TO_TCHAR(reinterpret_cast<char*>(&serializedMessage)));
			UE_LOG(LogTemp, Warning, TEXT("%s"), *logString);
		}
		FPlatformProcess::Sleep(1.0f / 60.0f);
	}

	return 0;
}

void SendDataWorker::Stop()
{
	stopTaskCounter.Increment();
}

void SendDataWorker::Exit()
{
	socket->Close();
}

ReceiveDataWorker::ReceiveDataWorker(FCriticalSection& mutex, FString myoDriverIP, uint32 port, TArray<uint8>& receiveData)
{
	this->mutex = &mutex;
	this->myoDriverIP = myoDriverIP;
	this->port = port;
	this->receiveData = &receiveData;
}

bool ReceiveDataWorker::Init()
{
	ipv4Address = TSharedPtr<FIPv4Address>(new FIPv4Address());
	FIPv4Address::Parse(myoDriverIP, *ipv4Address);
	socketSubSystem = TSharedPtr<ISocketSubsystem>(ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM));
	address = socketSubSystem->CreateInternetAddr(ipv4Address->GetValue(), port);
	socket = TSharedPtr<FSocket>(socketSubSystem->CreateSocket(NAME_DGram, TEXT("MyoSendSocket"), true));
	socket->SetNonBlocking(true);
	socket->SetReuseAddr(true);
	socket->Bind(*address);

	return true;
}

uint32 ReceiveDataWorker::Run()
{
	int32 receiveSent = 0;
	while (stopTaskCounter.GetValue() == 0)
	{
		TArray<uint8> receive;
		receive.Init(0, 58);
		receiveSent = 0;
		if (socket->RecvFrom(receive.GetData(), receive.Num(), receiveSent, *address))
		{
			auto receiveMessage = reinterpret_cast<char*>(receive.GetData());
			auto logString = FString("ReceiveMessage: ") + FString(receiveMessage);
			FScopeLock lock(mutex);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *logString);
			*receiveData = receive;
		}
		FPlatformProcess::Sleep(1.0f / 60.0f);
	}

	return 0;
}

void ReceiveDataWorker::Stop()
{
	stopTaskCounter.Increment();
}

void ReceiveDataWorker::Exit()
{
	socket->Close();
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
	FRotator rotation = FRotator::ZeroRotator;
	FVector acceleration = FVector::ZeroVector, gyro = FVector::ZeroVector;
	TArray<int8> emg;
	emg.Init(0, 8);
	MyoPose pose;
	MyoArm whichArm;
	MyoArmDirection xDirection;
	bool onPair = false, onConnect = false, onArmSync = false, onLock = true;
	{
		FScopeLock lock(&mutex);
		auto data = receiveData.GetData();
		id = (static_cast<uint64>(data[0]) << 0) | (static_cast<uint64>(data[1]) << 8) | (static_cast<uint64>(data[2]) << 16) | (static_cast<uint64>(data[3]) << 24) |
			(static_cast<uint64>(data[4]) << 32) | (static_cast<uint64>(data[5]) << 40) | (static_cast<uint64>(data[6]) << 48) | (static_cast<uint64>(data[7]) << 56);
		uint32 rx = (static_cast<uint32>(data[8]) << 0) | (static_cast<uint32>(data[9]) << 8) | (static_cast<uint32>(data[10]) << 16) | (static_cast<uint32>(data[11]) << 24);
		uint32 ry = (static_cast<uint32>(data[12]) << 0) | (static_cast<uint32>(data[13]) << 8) | (static_cast<uint32>(data[14]) << 16) | (static_cast<uint32>(data[15]) << 24);
		uint32 rz = (static_cast<uint32>(data[16]) << 0) | (static_cast<uint32>(data[17]) << 8) | (static_cast<uint32>(data[18]) << 16) | (static_cast<uint32>(data[19]) << 24);
		uint32 rw = (static_cast<uint32>(data[20]) << 0) | (static_cast<uint32>(data[21]) << 8) | (static_cast<uint32>(data[22]) << 16) | (static_cast<uint32>(data[23]) << 24);
		rotation = FRotator(FQuat(*reinterpret_cast<float*>(&rx), *reinterpret_cast<float*>(&ry), *reinterpret_cast<float*>(&rz), *reinterpret_cast<float*>(&rw)));
		uint32 ax = (static_cast<uint32>(data[24]) << 0) | (static_cast<uint32>(data[25]) << 8) | (static_cast<uint32>(data[26]) << 16) | (static_cast<uint32>(data[27]) << 24);
		uint32 ay = (static_cast<uint32>(data[28]) << 0) | (static_cast<uint32>(data[29]) << 8) | (static_cast<uint32>(data[30]) << 16) | (static_cast<uint32>(data[31]) << 24);
		uint32 az = (static_cast<uint32>(data[32]) << 0) | (static_cast<uint32>(data[33]) << 8) | (static_cast<uint32>(data[34]) << 16) | (static_cast<uint32>(data[35]) << 24);
		acceleration = FVector(*reinterpret_cast<float*>(&ax), *reinterpret_cast<float*>(&ay), *reinterpret_cast<float*>(&az));
		uint32 gx = (static_cast<uint32>(data[36]) << 0) | (static_cast<uint32>(data[37]) << 8) | (static_cast<uint32>(data[38]) << 16) | (static_cast<uint32>(data[39]) << 24);
		uint32 gy = (static_cast<uint32>(data[40]) << 0) | (static_cast<uint32>(data[41]) << 8) | (static_cast<uint32>(data[42]) << 16) | (static_cast<uint32>(data[43]) << 24);
		uint32 gz = (static_cast<uint32>(data[44]) << 0) | (static_cast<uint32>(data[45]) << 8) | (static_cast<uint32>(data[46]) << 16) | (static_cast<uint32>(data[47]) << 24);
		gyro = FVector(*reinterpret_cast<float*>(&gx), *reinterpret_cast<float*>(&gy), *reinterpret_cast<float*>(&gz));
		emg[0] = data[48];	emg[1] = data[49];	emg[2] = data[50];	emg[3] = data[51];
		emg[4] = data[52];	emg[5] = data[53];	emg[6] = data[54];	emg[7] = data[55];
		pose = static_cast<MyoPose>((data[56] & 0b11100000) >> 5);
		whichArm = static_cast<MyoArm>((data[56] & 0b00011100) >> 2);
		xDirection = static_cast<MyoArmDirection>((data[56] & 0b00000011) >> 0);
		onPair = ((data[57] & 0b00001000) >> 3) == 1 ? true : false;
		onConnect = ((data[57] & 0b00000100) >> 2) == 1 ? true : false;
		onArmSync = ((data[57] & 0b00000010) >> 1) == 1 ? true : false;
		onLock = ((data[57] & 0b00000001) >> 0) == 1 ? true : false;
	}
	onConnect ? OnConnect(id) : OnDisconnect(id);
	onArmSync ? OnArmSync(id, whichArm, xDirection) : OnArmUnsync(id);
	onPair ? OnPair(id) : OnUnpair(id);
	onLock ? OnLock(id) : OnUnlock(id);
	if (onConnect && onArmSync && onPair)
	{
		OnOrientationData(id, rotation);
		OnAccelerometerData(id, acceleration);
		OnGyroscopeData(id, gyro);
		OnPose(id, pose);
		OnEmgData(id, emg);
	}
}

void UDataCollector::OnConnect(uint64 myoId)
{

}

void UDataCollector::OnDisconnect(uint64 myoId)
{

}

void UDataCollector::OnArmSync(uint64 myoId, MyoArm arm, MyoArmDirection xDirection)
{

}

void UDataCollector::OnArmUnsync(uint64 myoId)
{

}

void UDataCollector::OnPair(uint64 myoId)
{

}

void UDataCollector::OnUnpair(uint64 myoId)
{

}

void UDataCollector::OnOrientationData(uint64 myoId, FRotator& rotation)
{

}

void UDataCollector::OnAccelerometerData(uint64 myoId, FVector& accel)
{

}

void UDataCollector::OnGyroscopeData(uint64 myoId, FVector& gyro)
{

}

void UDataCollector::OnUnlock(uint64 myoId)
{

}

void UDataCollector::OnLock(uint64 myoId)
{

}

void UDataCollector::OnPose(uint64 myoId, MyoPose pose)
{

}

void UDataCollector::OnEmgData(uint64 myoId, TArray<int8>& emg)
{

}

int32 UDataCollector::IdentifyMyo(uint64 myoId)
{
	return int32();
}

uint64 UDataCollector::LastValidMyo()
{
	return uint64();
}

bool UDataCollector::MyoIsValidForInputMapping(uint64 myoId)
{
	return false;
}

int32 UDataCollector::MyoIndexForMyo(uint64 myoId)
{
	return int32();
}

void UDataCollector::StartListening()
{

}

void UDataCollector::StopListening()
{

}

void UDataCollector::UnlockHoldEachMyo()
{

}

void UDataCollector::LockEachMyo()
{

}

bool UDataCollector::Startup()
{
	sendDataWorker = TSharedPtr<SendDataWorker>(new SendDataWorker(mutex, myoDriverIP, sendPort, sendData));
	receiveDataWorker = TSharedPtr<ReceiveDataWorker>(new ReceiveDataWorker(mutex, myoDriverIP, receivePort, receiveData));
	sendThread = TSharedPtr<FRunnableThread>(FRunnableThread::Create(sendDataWorker.Get(), TEXT("SendDataThread")));
	receiveThread = TSharedPtr<FRunnableThread>(FRunnableThread::Create(receiveDataWorker.Get(), TEXT("ReceiveDataThread")));

	return true;
}

void UDataCollector::ShutDown()
{
	if (sendDataWorker.Get() != nullptr)
		sendDataWorker->Stop();
	if (receiveDataWorker.Get() != nullptr)
		receiveDataWorker->Stop();
	if (sendThread.Get() != nullptr)
	{
		sendThread->WaitForCompletion();
		sendThread->Kill();
	}
	if (receiveThread.Get() != nullptr)
	{
		receiveThread->WaitForCompletion();
		receiveThread->Kill();
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

FRotator CombineRotators(FRotator a, FRotator b)
{
	return FRotator(FQuat(b) * FQuat(a));
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
	auto fullCompensationOrientation = CombineRotators(orientation, armYawCorrection);
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
	auto tempRot = CombineRotators(FRotator(0.0f, 0.0f, armCorrection.Roll*directionModifier), convertedOrientation);
	return CombineRotators(tempRot, FRotator(0.0f, armCorrection.Yaw, 0.0f));
}
