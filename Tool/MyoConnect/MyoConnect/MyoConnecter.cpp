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
	if (hub == nullptr)
	{
		std::cout << LogString << ErrorString << "hub‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B" << std::endl;
		return false;
	}
	hub->addListener(collector);
	auto myo = hub->waitForMyo(10000);
	myo->setStreamEmg(myo::Myo::streamEmgEnabled);
	collector->KnownMyos.push_back(myo);
	collector->MyoInfos.push_back(MyoInformation());
	collector->MyoInfos[0].MyoPtr = myo;
	collector->MyoInfos[0].OnPair = true;
	if (!udpSocket->Initialize())
		return false;

	return true;
}

bool MyoConnecter::Update()
{
	if (GetInputEscapeKey())
		return false;

	hub->run(1000 / FPS);
	for (auto myo : collector->MyoInfos)
	{
		udpSocket->SetSendMessage(myo);
		//collector->PrintData(myo);
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
			std::cout << LogString << "ESC‚ª‰Ÿ‚³‚ê‚Ü‚µ‚½B" << std::endl;
			udpSocket->SetIsFinish(true);
			return true;
		}
	}
	return false;
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
