#pragma once

#include <string>

struct WSAData;
struct sockaddr_in; 

class UDPSocket
{
public:
	UDPSocket();
	~UDPSocket();
	bool Initialize();

private:
	WSAData* winSocketApiData;
	unsigned long long sock;
	sockaddr_in* addressIn;
	sockaddr_in* from;
	char recieveBuffer[256];
	char sendBuffer[256];

	static const std::string LogString;
	static const std::string ErrorString;

	static bool WindowSocketAPIStartup(WSAData& winSocketAPIData);
	static bool CreateSocket(unsigned long long& sock);
	static bool BindSocket(unsigned long long& sock, sockaddr_in& addressIn, const char* ipv4Address, const int port = 8000);
	static void CloseSocket(unsigned long long& sock);
	static void WindowSocketAPICleanup();
};

