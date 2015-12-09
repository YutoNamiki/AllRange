#pragma once

#include <string>
#include <myo/Myo.hpp>

class UDPSocket;
class DataCollector;

class MyoConnecter
{
public:
	MyoConnecter();
	~MyoConnecter();
	static void Run();

private:
	UDPSocket* udpSocket;
	DataCollector* collector;
	myo::Hub hub;
	
	static const std::string LogString;
	static const std::string ErrorString;

	bool Initializer();
	bool Update();

	bool GetInputEscapeKey();
};
