#pragma once

#include <string>

class UDPSocket;

class MyoConnecter
{
public:
	MyoConnecter();
	~MyoConnecter();
	static void Run();

private:
	UDPSocket* udpSocket;
	
	static const std::string LogString;
	static const std::string ErrorString;

	bool Initializer();
	bool Update();

	bool GetInputEscapeKey();
};
