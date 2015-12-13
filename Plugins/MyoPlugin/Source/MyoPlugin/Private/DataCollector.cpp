#include "MyoPluginPrivatePCH.h"
#include "DataCollector.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

SendDataWorker::SendDataWorker(FString myoDriverIP, uint32 port)
{
	this->myoDriverIP = myoDriverIP;
	this->port = port;
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

ReceiveDataWorker::ReceiveDataWorker(FString myoDriverIP, uint32 port)
{
	this->myoDriverIP = myoDriverIP;
	this->port = port;
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
		TArray<uint8> receiveData;
		receiveData.Init(0, 58);
		receiveSent = 0;
		if (socket->RecvFrom(receiveData.GetData(), receiveData.Num(), receiveSent, *address))
		{
			auto receiveMessage = reinterpret_cast<char*>(receiveData.GetData());
			auto logString = FString("ReceiveMessage: ") + FString(receiveMessage);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *logString);
		}
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

bool UDataCollector::Startup()
{
	sendDataWorker = TSharedPtr<SendDataWorker>(new SendDataWorker(MyoDriverIP, SendPort));
	receiveDataWorker = TSharedPtr<ReceiveDataWorker>(new ReceiveDataWorker(MyoDriverIP, ReceivePort));
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
