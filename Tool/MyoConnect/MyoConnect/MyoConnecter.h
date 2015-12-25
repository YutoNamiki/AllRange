#pragma once

#include <memory>
#include <mutex>
#include <string>

class DataCollector;
class DataTransceiver;

namespace std
{
	class mutex;
}
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
	std::shared_ptr<DataCollector> collector;
	std::shared_ptr<DataTransceiver> transceiver;
	std::shared_ptr<myo::Hub> hub;
	std::shared_ptr<std::mutex> mutex;
	
	static const int FPS = 90;
	static const std::string LogString;
	static const std::string ErrorString;

	bool Initializer();
	bool Update();

	bool GetInputEscapeKey();
};
