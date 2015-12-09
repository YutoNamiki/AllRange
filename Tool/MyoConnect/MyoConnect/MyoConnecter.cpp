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
	std::cout << LogString << "ソケットをクローズしました。" << std::endl;
	WSACleanup();
	std::cout << LogString << "WinSocketAPIをクリーンアップしました。" << std::endl;
	std::cout << LogString << "アプリケーションを終了します。" << std::endl;
}

bool MyoConnecter::Initializer()
{
	if (WSAStartup(MAKEWORD(2, 0), &winSocketApiData) != 0)
	{
		std::cout << LogString << ErrorString << "WinSocketAPIの初期化に失敗しました。" << std::endl;
		return false;
	}
	std::cout << LogString << "WinSocketAPIを初期化しました。" << std::endl;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		std::cout << LogString << ErrorString << "ソケットの初期化に失敗しました。" << std::endl;
		return false;
	}
	std::cout << LogString << "ソケットを初期化しました。" << std::endl;
	
	memset(&addressIn, 0, sizeof(addressIn));
	addressIn.sin_port = htons(8000);
	addressIn.sin_family = AF_INET;
	addressIn.sin_addr.s_addr = htonl(INADDR_ANY);
	nRtn = bind(sock, reinterpret_cast<LPSOCKADDR>(&addressIn), static_cast<int>(sizeof(addressIn)));
	if (nRtn == SOCKET_ERROR)
	{
		std::cout << LogString << ErrorString << "ソケットのバインドに失敗しました。" << std::endl;
		return false;
	}
	std::cout << LogString << "ソケットをバインドしました。" << std::endl;
	return true;
}

bool MyoConnecter::Update()
{
	if (_kbhit())
	{
		auto key = _getch();
		if (key == 0x1b)
		{
			std::cout << LogString << "ESCが押されました。アプリケーションを終了します。" << std::endl;
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
