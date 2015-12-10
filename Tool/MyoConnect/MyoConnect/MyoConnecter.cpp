#include "MyoConnecter.h"
#include <iostream>
#include <conio.h>
#include <myo/Myo.hpp>
#include "DataCollector.h"
#include "UDPSocket.h"

#pragma comment(lib, "kernel32.lib")

union IntAndFloat
{
	int Integer;
	float Float;
};

const std::string MyoConnecter::LogString = std::string("MyoConnect: ");
const std::string MyoConnecter::ErrorString = std::string("Error: ");

MyoConnecter::MyoConnecter()
{
	hub = new myo::Hub("com.tool.myoconnect");
	collector = new DataCollector();
	udpSocket = new UDPSocket();
}

MyoConnecter::~MyoConnecter()
{
	hub->removeListener(collector);
	delete udpSocket;
	delete collector;
	delete hub;
}

bool MyoConnecter::Initializer()
{
	hub->addListener(collector);
	auto myo = hub->waitForMyo(10000);
	myo->setStreamEmg(myo::Myo::streamEmgEnabled);
	collector->KnownMyos.push_back(myo);
	collector->MyoInfos.push_back(MyoInformation());
	collector->MyoInfos[0].OnPair = true;
	if (!udpSocket->Initialize())
		return false;

	return true;
}

bool MyoConnecter::Update()
{
	if (GetInputEscapeKey())
		return false;

	hub->run(1000 / 20);
	for (auto knownMyo : collector->KnownMyos)
	{
		collector->PrintData(collector->MyoInfos[collector->IdentifyMyo(knownMyo)]);
		int message[256];
		SetMessage(knownMyo, *collector, message);
	}
	return true;
}

bool MyoConnecter::GetInputEscapeKey()
{
	if (_kbhit())
	{
		auto key = _getch();
		if (key == 0x1b)
		{
			std::cout << LogString << "ESCが押されました。" << std::endl;
			return true;
		}
	}
	return false;
}

void MyoConnecter::SetMessage(myo::Myo* sendMyo, DataCollector& collector, int* message)
{
	auto myo = collector.MyoInfos[collector.IdentifyMyo(sendMyo)];
	/* ビットは下位からシフト
	[0]: 4 Byte - Myo Pointer
	[1]: 4 Byte - Rotation X
	[2]: 4 Byte - Rotation Y
	[3]: 4 Byte - Rotation Z
	[4]: 4 Byte - Rotation W
	[5]: 4 Byte - Acceleration X
	[6]: 4 Byte - Acceleration Y
	[7]: 4 Byte - Acceleration Z
	[8]: 4 Byte - Gyro X
	[9]: 4 Byte - Gyro Y
	[10]: 4 Byte - Gyro Z
	[11]: 1 Byte - Emg[0]
		  1 Byte - Emg[1]
		  1 Byte - Emg[2]
		  1 Byte - Emg[3]
	[12]: 1 Byte - Emg[4]
		  1 Byte - Emg[5]
		  1 Byte - Emg[6]
		  1 Byte - Emg[7]
	[13]: 1 Bit - OnPair (0:false, 1:true)
		  1 Bit - OnConnect (0:false, 1:true)
	      1 Bit - OnArmSync (0:false, 1:true)
		  1 Bit - OnLock (0:false, 1:true)
		  2 Bit - WhichArm (0:right, 1:left, 2:unknown)
		  2 Bit - ArmDirection (0:wrist, 1:elbow, 2:unknown)
		  3 Bit - Pose(0:rest, 1:fist, 2:waveIn, 3:waveOut, 4:fingersSpread, 5:doubleTap, 6:unknown)
	*/
	std::cout << LogString << "Myo Ptr: " << std::hex << sendMyo << std::dec << std::endl;
	message[0] = reinterpret_cast<int>(sendMyo);
	auto address = reinterpret_cast<myo::Myo*>(message[0]);
	std::cout << LogString << "Myo Ptr: " << std::hex << address << std::dec << std::endl;
}

void MyoConnecter::Run()
{
	auto connecter = new MyoConnecter();
	if (!connecter->Initializer())
	{
		delete connecter;
		return;
	}
	while (connecter->Update());
	delete connecter;
}
