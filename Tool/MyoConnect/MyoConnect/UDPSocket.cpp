#define WIN32_LEAN_AND_MEAN

#include "UDPSocket.h"
#include <WS2tcpip.h>

const std::string UDPSocket::LogString = std::string("UDPSocket: ");
const std::string UDPSocket::ErrorString = std::string("Error: ");

UDPSocket::UDPSocket()
{
	winSocketApiData = new WSAData();
	Address = new sockaddr_in();
}

UDPSocket::~UDPSocket()
{
	CloseSocket(Sock);
	WindowSocketAPICleanup();
	delete Address;
	delete winSocketApiData;
	std::cout << LogString << "�A�v���P�[�V�������I�����܂��B" << std::endl;
}

bool UDPSocket::Initialize()
{
	if (!WindowSocketAPIStartup(*winSocketApiData))
		return false;
	if (!CreateSocket(Sock))
		return false;
	char ipv4Buffer[256];
	std::cout << "�g�p����PC��IPv4�A�h���X����͂��Ă��������B(***.***.***.***)�F";
	std::cin >> ipv4Buffer;
	if (!BindSocket(Sock, *Address, ipv4Buffer))
	{
		char portBuffer[256];
		std::cout << LogString << ErrorString << "�|�[�g�ԍ�8000�ł̃\�P�b�g�̃o�C���h�Ɏ��s���܂����B" << std::endl;
		std::cout << "8000�ԈȊO�Ŏg�p����|�[�g�ԍ�����͂��Ă��������F";
		std::cin >> portBuffer;
		if (!BindSocket(Sock, *Address, ipv4Buffer, atoi(portBuffer)))
			return false;
	}

	return true;
}

void UDPSocket::SendMessageTo(SOCKET& sock, char* buffer, size_t bufferSize, sockaddr_in& addressIn)
{
	auto result = sendto(sock, buffer, static_cast<int>(bufferSize), 0, reinterpret_cast<sockaddr*>(&addressIn), static_cast<int>(sizeof(addressIn)));
	if (result == SOCKET_ERROR)
	{
		std::cout << LogString << ErrorString << "�L�����u���[�V�������̑��M�Ɏ��s���܂����B" << std::endl;
		return;
	}
	//std::cout << LogString << "�L�����u���[�V�������𑗐M���܂����B" << std::endl;
}

void UDPSocket::ReceiveMessageFrom(SOCKET& sock, char* buffer, size_t bufferSize, sockaddr_in& addressFrom)
{
	auto fromSize = static_cast<int>(sizeof(addressFrom));
	auto result = recvfrom(sock, buffer, static_cast<int>(bufferSize), 0, reinterpret_cast<sockaddr*>(&addressFrom), &fromSize);
	if (result == SOCKET_ERROR)
	{
		std::cout << LogString << ErrorString << "�A�v���P�[�V��������̃p�P�b�g�����X���܂����B" << std::endl;
		return;
	}
	std::cout << LogString << ErrorString << "�A�v���P�[�V��������̃p�P�b�g���擾���܂����B" << std::endl;
}

bool UDPSocket::WindowSocketAPIStartup(WSAData& winSocketAPIData)
{
	if (WSAStartup(MAKEWORD(2, 0), &winSocketAPIData) != 0)
	{
		std::cout << LogString << ErrorString << "WinSocketAPI�̏������Ɏ��s���܂����B" << std::endl;
		return false;
	}
	std::cout << LogString << "WinSocketAPI�����������܂����B" << std::endl;
	return true;
}

bool UDPSocket::CreateSocket(SOCKET& sock)
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		std::cout << LogString << ErrorString << "�\�P�b�g�̏������Ɏ��s���܂����B" << std::endl;
		return false;
	}
	std::cout << LogString << "�\�P�b�g�����������܂����B" << std::endl;
	return true;
}

bool UDPSocket::BindSocket(SOCKET& sock, sockaddr_in& Address, const char* ipv4Address, const int port)
{
	memset(&Address, 0, sizeof(Address));
	Address.sin_port = htons(port);
	Address.sin_family = AF_INET;
	inet_pton(AF_INET, ipv4Address, &(Address.sin_addr));
	if (bind(sock, reinterpret_cast<LPSOCKADDR>(&Address), static_cast<int>(sizeof(Address))) == SOCKET_ERROR)
	{
		std::cout << LogString << ErrorString << "�\�P�b�g�̃o�C���h�Ɏ��s���܂����B" << std::endl;
		return false;
	}
	std::cout << LogString << "�\�P�b�g���o�C���h���܂����B" << std::endl;
	return true;
}

void UDPSocket::CloseSocket(SOCKET& sock)
{
	closesocket(sock);
	std::cout << LogString << "�\�P�b�g���N���[�Y���܂����B" << std::endl;
}

void UDPSocket::WindowSocketAPICleanup()
{
	WSACleanup();
	std::cout << LogString << "WinSocketAPI���N���[���A�b�v���܂����B" << std::endl;
}
