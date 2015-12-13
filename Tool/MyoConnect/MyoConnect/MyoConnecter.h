#pragma once

#include <string>

class UDPSocket;
class DataCollector;

namespace myo
{
	class Hub;
	class Myo;
	template <typename T> class Quaternion;
	template <typename T> class Vector3;
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
	
	static const int FPS = 90;
	static const std::string LogString;
	static const std::string ErrorString;

	bool Initializer();
	bool Update();

	bool GetInputEscapeKey();
};
