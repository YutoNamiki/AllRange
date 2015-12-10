#define WIN32_LEAN_AND_MEAN

#include "UDPSocket.h"
#include <WS2tcpip.h>

const std::string UDPSocket::LogString = std::string("UDPSocket: ");
const std::string UDPSocket::ErrorString = std::string("Error: ");

UDPSocket::UDPSocket()
{
	winSocketApiData = new WSAData();
	AddressIn = new sockaddr_in();
	From = new sockaddr_in();
}

UDPSocket::~UDPSocket()
{
	CloseSocket(Sock);
	WindowSocketAPICleanup();
	delete From;
	delete AddressIn;
	delete winSocketApiData;
	std::cout << LogString << "アプリケーションを終了します。" << std::endl;
}

bool UDPSocket::Initialize()
{
	if (!WindowSocketAPIStartup(*winSocketApiData))
		return false;
	if (!CreateSocket(Sock))
		return false;
	char ipv4Buffer[256];
	std::cout << "使用するPCのIPv4アドレスを入力してください。(***.***.***.***)：";
	std::cin >> ipv4Buffer;
	if (!BindSocket(Sock, *AddressIn, ipv4Buffer))
	{
		char portBuffer[256];
		std::cout << LogString << ErrorString << "ポート番号8000でのソケットのバインドに失敗しました。" << std::endl;
		std::cout << "8000番以外で使用するポート番号を入力してください：";
		std::cin >> portBuffer;
		if (!BindSocket(Sock, *AddressIn, ipv4Buffer, atoi(portBuffer)))
			return false;
	}

	return true;
}

void UDPSocket::SendMessageTo(SOCKET& sock, char* buffer, size_t bufferSize, sockaddr_in& addressIn)
{
	auto result = sendto(sock, buffer, static_cast<int>(bufferSize), 0, reinterpret_cast<sockaddr*>(&addressIn), static_cast<int>(sizeof(addressIn)));
	if (result == SOCKET_ERROR)
	{
		std::cout << LogString << ErrorString << "キャリブレーション情報の送信に失敗しました。" << std::endl;
		return;
	}
	//std::cout << LogString << "キャリブレーション情報を送信しました。" << std::endl;
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

bool UDPSocket::CreateSocket(SOCKET& sock)
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

bool UDPSocket::BindSocket(SOCKET& sock, sockaddr_in& addressIn, const char* ipv4Address, const int port)
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

void UDPSocket::CloseSocket(SOCKET& sock)
{
	closesocket(sock);
	std::cout << LogString << "ソケットをクローズしました。" << std::endl;
}

void UDPSocket::WindowSocketAPICleanup()
{
	WSACleanup();
	std::cout << LogString << "WinSocketAPIをクリーンアップしました。" << std::endl;
}
