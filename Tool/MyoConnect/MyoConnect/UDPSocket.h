#pragma once

#include <iostream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")

class UDPSocket
{
public:
	SOCKET Sock;
	sockaddr_in* Address;

	UDPSocket();
	~UDPSocket();
	bool Initialize();
	static void SendMessageTo(SOCKET& sock, char* buffer, size_t bufferSize, sockaddr_in& addressIn);
	static void ReceiveMessageFrom(SOCKET& sock, char* buffer, size_t bufferSize, sockaddr_in& addressFrom);

private:
	WSAData* winSocketApiData;

	static const std::string LogString;
	static const std::string ErrorString;

	static bool WindowSocketAPIStartup(WSAData& winSocketAPIData);
	static bool CreateSocket(SOCKET& sock);
	static bool BindSocket(SOCKET& sock, sockaddr_in& addressIn, const char* ipv4Address, const int port = 8000);
	static void CloseSocket(SOCKET& sock);
	static void WindowSocketAPICleanup();


};
