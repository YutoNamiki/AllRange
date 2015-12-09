#include "MyoConnecter.h"
#include <iostream>
#include <conio.h>
#include <Windows.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "kernel32.lib")

const std::string MyoConnecter::LogString = std::string("MyoConnect: ");
const std::string MyoConnecter::ErrorString = std::string("Error: ");

MyoConnecter::MyoConnecter()
{
	
}

MyoConnecter::~MyoConnecter()
{
	closesocket(sock);
	std::cout << LogString << "�\�P�b�g���N���[�Y���܂����B" << std::endl;
	WSACleanup();
	std::cout << LogString << "WinSocketAPI���N���[���A�b�v���܂����B" << std::endl;
	std::cout << LogString << "�A�v���P�[�V�������I�����܂��B" << std::endl;
}

bool MyoConnecter::Initializer()
{
	if (WSAStartup(MAKEWORD(2, 0), &winSocketApiData) != 0)
	{
		std::cout << LogString << ErrorString << "WinSocketAPI�̏������Ɏ��s���܂����B" << std::endl;
		return false;
	}
	std::cout << LogString << "WinSocketAPI�����������܂����B" << std::endl;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		std::cout << LogString << ErrorString << "�\�P�b�g�̏������Ɏ��s���܂����B" << std::endl;
		return false;
	}
	std::cout << LogString << "�\�P�b�g�����������܂����B" << std::endl;
	
	memset(&addressIn, 0, sizeof(addressIn));
	addressIn.sin_port = htons(8000);
	addressIn.sin_family = AF_INET;
	addressIn.sin_addr.s_addr = htonl(INADDR_ANY);
	nRtn = bind(sock, reinterpret_cast<LPSOCKADDR>(&addressIn), static_cast<int>(sizeof(addressIn)));
	if (nRtn == SOCKET_ERROR)
	{
		std::cout << LogString << ErrorString << "�\�P�b�g�̃o�C���h�Ɏ��s���܂����B" << std::endl;
		return false;
	}
	std::cout << LogString << "�\�P�b�g���o�C���h���܂����B" << std::endl;
	return true;
}

bool MyoConnecter::Update()
{
	if (_kbhit())
	{
		auto key = _getch();
		if (key == 0x1b)
		{
			std::cout << LogString << "ESC��������܂����B�A�v���P�[�V�������I�����܂��B" << std::endl;
			return false;
		}
	}
	fromLength = static_cast<int>(sizeof(from));
	

	Sleep(1000 / 90);
	return true;
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
