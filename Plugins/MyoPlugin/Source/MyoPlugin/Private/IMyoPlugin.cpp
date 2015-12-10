// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyoPluginPrivatePCH.h"
#include "MyoPluginObject.h"
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

class FMyoPlugin : public IMyoPlugin
{
public:
	FString MyoDriverIP = "192.168.10.5";
	FIPv4Address MyoDirverIPAddress;
	FSocket* Socket;
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
	Socket = FUdpSocketBuilder(TEXT("MyoUDPSocket")).BoundToAddress(MyoDirverIPAddress).BoundToPort(8000).Build();
	//Receiver = new FUdpSocketReceiver(Socket, FTimespan(0, 1, 0), TEXT("MyoUDPReceiver"));
	uint32 pendingData;
	if (Socket->HasPendingData(pendingData))
	{
		uint8* recv = new uint8[pendingData];
		int32 bytesRead;
		TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		Socket->RecvFrom(recv, pendingData, bytesRead, *targetAddr);
		auto character = (char*)recv;
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Red, FString(character));
	}
}

void FMyoPlugin::ShutdownModule()
{
	delete Receiver;
	Socket->Close();
}

void FMyoPlugin::ReceivePacket()
{

}

void FMyoPlugin::SendPacket()
{

}
