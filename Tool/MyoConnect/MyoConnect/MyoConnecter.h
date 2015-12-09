#pragma once

#include <WinSock2.h>
#include <string>

class MyoConnecter
{
public:
	MyoConnecter();
	~MyoConnecter();
	static void Run();

private:
	WSAData winSocketApiData;
	SOCKET sock;
	sockaddr_in addressIn;
	sockaddr_in from;
	int fromLength;
	int nRtn;
	u_short port;
	char recieveBuffer[256];
	char sendBuffer[256];

	static const std::string LogString;
	static const std::string ErrorString;

	bool Initializer();
	bool Update();
};
