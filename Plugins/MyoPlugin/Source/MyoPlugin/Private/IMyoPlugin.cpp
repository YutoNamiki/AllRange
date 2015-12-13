// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyoPluginPrivatePCH.h"
#include "MyoPluginObject.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

class FMyoPlugin : public IMyoPlugin
{
public:
	FString MyoDriverIP = "127.0.0.1";
	uint32 SendPort = 8000;
	uint32 ReceivePort = 8001;
	FIPv4Address MyoDirverIPAddress;
	FSocket* SendSocket;
	FSocket* ReceiveSocket;
	FUdpSocketReceiver* Receiver;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void ReceivePacket();
	void SendPacket();
};

IMPLEMENT_MODULE(FMyoPlugin, MyoPlugin )

void FMyoPlugin::StartupModule()
{
	FIPv4Address::Parse(MyoDriverIP, MyoDirverIPAddress);
	auto socketSubSystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	auto sendAddress = socketSubSystem->CreateInternetAddr(MyoDirverIPAddress.GetValue(), SendPort);
	auto receiveAddress = socketSubSystem->CreateInternetAddr(MyoDirverIPAddress.GetValue(), ReceivePort);
	
	SendSocket = socketSubSystem->CreateSocket(NAME_DGram, TEXT("MyoSendSocket"), true);
	SendSocket->SetNonBlocking(true);
	SendSocket->SetReuseAddr(true);
	
	ReceiveSocket = socketSubSystem->CreateSocket(NAME_DGram, TEXT("MyoReceiveSocket"), true);
	//ReceiveSocket->SetNonBlocking(true);
	ReceiveSocket->SetReuseAddr(true);
	ReceiveSocket->Bind(*receiveAddress);
	FString message = "Hello, Myo!";
	TCHAR* charMessage = message.GetCharArray().GetData();
	char* utfMessage = TCHAR_TO_UTF8(charMessage);
	int32 sendSize = FCString::Strlen(charMessage);
	auto serializedMessage = reinterpret_cast<uint8*>(utfMessage);
	int32 sendSent = 0;
	if (SendSocket->SendTo(serializedMessage, sendSize, sendSent, *sendAddress))
	{
		//char* reverseMessage = reinterpret_cast<char*>(&serializedMessage);
		TCHAR* reverseTMessage = UTF8_TO_TCHAR(serializedMessage);
		auto logString = FString("SendMessage: ") + FString(reverseTMessage);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *logString);
	}
	TArray<uint8> receiveData;
	receiveData.Init(0, 256);
	int32 receiveSent = 0;
	if (ReceiveSocket->RecvFrom(receiveData.GetData(), receiveData.Num(), receiveSent, *receiveAddress))
	{
		auto receiveMessage = reinterpret_cast<char*>(receiveData.GetData());
		auto logString = FString("ReceiveMessege: ") + FString(receiveMessage);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *logString);
	}
}

void FMyoPlugin::ShutdownModule()
{
	delete Receiver;
	if (SendSocket != nullptr)
		SendSocket->Close();
	if (ReceiveSocket != nullptr)
		ReceiveSocket->Close();
}

void FMyoPlugin::ReceivePacket()
{

}

void FMyoPlugin::SendPacket()
{

}
