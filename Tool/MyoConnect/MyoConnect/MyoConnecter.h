#pragma once

#include <string>

class UDPSocket;
class DataCollector;
namespace myo
{
	class Hub;
	class Myo;
}

class MyoConnecter
{
public:
	MyoConnecter();
	~MyoConnecter();
	static void Run();

private:
	UDPSocket* udpSocket;
	DataCollector* collector;
	myo::Hub* hub;
	
	static const std::string LogString;
	static const std::string ErrorString;

	bool Initializer();
	bool Update();

	bool GetInputEscapeKey();
	static void SetMessage(myo::Myo* sendMyo, DataCollector& collector, int* message);
};
