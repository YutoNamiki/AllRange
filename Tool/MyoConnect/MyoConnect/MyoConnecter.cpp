#include "MyoConnecter.h"
#include <mutex>
#include <iostream>
#include <conio.h>
#include <myo/Myo.hpp>
#include "DataCollector.h"
#include "DataTransceiver.h"

#pragma comment(lib, "kernel32.lib")

const std::string MyoConnecter::LogString = std::string("MyoConnect: ");
const std::string MyoConnecter::ErrorString = std::string("Error: ");

MyoConnecter::MyoConnecter()
{
	mutex = std::make_shared<std::mutex>();
	hub = std::make_shared<myo::Hub>("com.tool.myoconnect");
	collector = std::make_shared<DataCollector>(mutex);
	transceiver = std::make_shared<DataTransceiver>(mutex);
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
	hub->setLockingPolicy(myo::Hub::LockingPolicy::lockingPolicyNone);
	auto myo = hub->waitForMyo(10000);
	myo->setStreamEmg(myo::Myo::streamEmgEnabled);
	collector->KnownMyos.push_back(myo);
	collector->MyoInfos.push_back(MyoInformation());
	collector->MyoInfos[0].MyoPtr = myo;
	collector->MyoInfos[0].OnPair = true;
	if (!transceiver->Initialize())
		return false;

	mutex->lock();
	std::cout << LogString << "MyoConnecterを正常に起動しました。" << std::endl;
	std::cout << LogString << "終了したい場合はEscキーを押してください。" << std::endl;
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
		transceiver->SetSendMessage(myo);
		transceiver->GetReceiveMessage(hub, collector->KnownMyos);
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
			transceiver->SetIsFinish(true);
			return true;
		}
		else if (key == 0x52 || key == 0x72)
		{
			mutex->lock();
			system("cls");
			std::cout << LogString << "ログをリフレッシュしました。" << std::endl;
			std::cout << LogString << "終了したい場合はEscキーを押してください。" << std::endl;
			mutex->unlock();
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
