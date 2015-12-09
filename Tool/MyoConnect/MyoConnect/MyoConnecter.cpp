#include "MyoConnecter.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "UDPSocket.h"

#pragma comment(lib, "kernel32.lib")

const std::string MyoConnecter::LogString = std::string("MyoConnect: ");
const std::string MyoConnecter::ErrorString = std::string("Error: ");

MyoConnecter::MyoConnecter()
{
	udpSocket = new UDPSocket();
}

MyoConnecter::~MyoConnecter()
{
	delete udpSocket;
}

bool MyoConnecter::Initializer()
{
	if (!udpSocket->Initialize())
		return false;

	return true;
}

bool MyoConnecter::Update()
{
	if (GetInputEscapeKey())
		return false;

	Sleep(1000 / 90);
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
