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
	std::cout << LogString << "�A�v���P�[�V�������I�����܂��B" << std::endl;
	mutex->unlock();
}

bool MyoConnecter::Initializer()
{
	if (hub == nullptr)
	{
		std::cout << LogString << ErrorString << "hub�̐����Ɏ��s���܂����B" << std::endl;
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
	std::cout << LogString << "MyoConnecter�𐳏�ɋN�����܂����B" << std::endl;
	std::cout << LogString << "�I���������ꍇ��Esc�L�[�������Ă��������B" << std::endl;
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
			std::cout << LogString << "ESC��������܂����B" << std::endl;
			mutex->unlock();
			transceiver->SetIsFinish(true);
			return true;
		}
		else if (key == 0x52 || key == 0x72)
		{
			mutex->lock();
			system("cls");
			std::cout << LogString << "���O�����t���b�V�����܂����B" << std::endl;
			std::cout << LogString << "�I���������ꍇ��Esc�L�[�������Ă��������B" << std::endl;
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
