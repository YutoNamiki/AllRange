#include "UDPSocket.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")

const std::string UDPSocket::LogString = std::string("UDPSocket: ");
const std::string UDPSocket::ErrorString = std::string("Error: ");

UDPSocket::UDPSocket()
{
	winSocketApiData = new WSAData();
	addressIn = new sockaddr_in();
	from = new sockaddr_in();
}

UDPSocket::~UDPSocket()
{
	CloseSocket(sock);
	WindowSocketAPICleanup();
	delete from;
	delete addressIn;
	delete winSocketApiData;
	std::cout << LogString << "アプリケーションを終了します。" << std::endl;
}

bool UDPSocket::Initialize()
{
	if (!WindowSocketAPIStartup(*winSocketApiData))
		return false;
	if (!CreateSocket(sock))
		return false;
	char ipv4Buffer[256];
	std::cout << "使用するPCのIPv4アドレスを入力してください。(***.***.***.***)：";
	std::cin >> ipv4Buffer;
	if (!BindSocket(sock, *addressIn, ipv4Buffer))
	{
		char portBuffer[256];
		std::cout << LogString << ErrorString << "ポート番号8000でのソケットのバインドに失敗しました。" << std::endl;
		std::cout << "8000番以外で使用するポート番号を入力してください：";
		std::cin >> portBuffer;
		if (!BindSocket(sock, *addressIn, ipv4Buffer, atoi(portBuffer)))
			return false;
	}

	return true;
}

bool UDPSocket::WindowSocketAPIStartup(WSAData& winSocketAPIData)
{
	if (WSAStartup(MAKEWORD(2, 0), &winSocketAPIData) != 0)
	{
		std::cout << LogString << ErrorString << "WinSocketAPIの初期化に失敗しました。" << std::endl;
		return false;
	}
	std::cout << LogString << "WinSocketAPIを初期化しました。" << std::endl;
	return true;
}

bool UDPSocket::CreateSocket(unsigned long long& sock)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		std::cout << LogString << ErrorString << "ソケットの初期化に失敗しました。" << std::endl;
		return false;
	}
	std::cout << LogString << "ソケットを初期化しました。" << std::endl;
	return true;
}

bool UDPSocket::BindSocket(unsigned long long& sock, sockaddr_in& addressIn, const char* ipv4Address, const int port)
{
	memset(&addressIn, 0, sizeof(addressIn));
	addressIn.sin_port = htons(port);
	addressIn.sin_family = AF_INET;
	inet_pton(AF_INET, ipv4Address, &(addressIn.sin_addr));
	if (bind(sock, reinterpret_cast<LPSOCKADDR>(&addressIn), static_cast<int>(sizeof(addressIn))) == SOCKET_ERROR)
	{
		std::cout << LogString << ErrorString << "ソケットのバインドに失敗しました。" << std::endl;
		return false;
	}
	std::cout << LogString << "ソケットをバインドしました。" << std::endl;
	return true;
}

void UDPSocket::CloseSocket(unsigned long long& sock)
{
	closesocket(sock);
	std::cout << LogString << "ソケットをクローズしました。" << std::endl;
}

void UDPSocket::WindowSocketAPICleanup()
{
	WSACleanup();
	std::cout << LogString << "WinSocketAPIをクリーンアップしました。" << std::endl;
}

template<typename T>
void UDPSocket::SendMessageTo(const unsigned long long& sock, T* buffer, size_t bufferSize, sockaddr_in addressIn)
{
	auto result = sendto(sock, buffer, static_cast<int>(bufferSize), 0, reinterpret_cast<LPSOCKADDR>(&addressIn), static_cast<int>(sizeof(addressIn)));
	if (result != static_cast<int>(strlen(buffer) + 1))
	{
		std::cout << LogString << ErrorString << "キャリブレーション情報の送信に失敗しました。" << std::endl;
		return;
	}
	std::cout << LogString << "キャリブレーション情報を送信しました。" << std::endl;
}
