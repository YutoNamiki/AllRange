#include "MyoConnecter.h"
#include <mutex>
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
	mutex = std::make_shared<std::mutex>();
	hub = std::make_shared<myo::Hub>("com.tool.myoconnect");
	collector = std::make_shared<DataCollector>(mutex);
	udpSocket = std::make_shared<UDPSocket>(mutex);
}

MyoConnecter::~MyoConnecter()
{
	hub->removeListener(collector.get());
	mutex->lock();
	std::cout << LogString << "アプリケーションを終了します。" << std::endl;
	mutex->unlock();
}

bool MyoConnecter::Initializer()
{
	if (hub == nullptr)
	{
		std::cout << LogString << ErrorString << "hubの生成に失敗しました。" << std::endl;
		return false;
	}
	hub->addListener(collector.get());
	auto myo = hub->waitForMyo(10000);
	myo->setStreamEmg(myo::Myo::streamEmgEnabled);
	collector->KnownMyos.push_back(myo);
	collector->MyoInfos.push_back(MyoInformation());
	collector->MyoInfos[0].MyoPtr = myo;
	collector->MyoInfos[0].OnPair = true;
	if (!udpSocket->Initialize())
		return false;

	mutex->lock();
	std::cout << LogString << ErrorString << "MyoConnecterを正常に起動しました。" << std::endl;
	std::cout << LogString << ErrorString << "終了したい場合はEscキーを押してください。" << std::endl;
	mutex->unlock();
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
			mutex->lock();
			std::cout << LogString << "ESCが押されました。" << std::endl;
			mutex->unlock();
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
