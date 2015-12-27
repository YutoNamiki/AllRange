#pragma once

#include <array>
#include <memory>

struct MyoInformation;

namespace std
{
	class mutex;
	class thread;
}

struct InputInformation
{
	void* Ptr;
	float RotationX;
	float RotationY;
	float RotationZ;
	float RotationW;
	float AccelerationX;
	float AccelerationY;
	float AccelerationZ;
	float GyroX;
	float GyroY;
	float GyroZ;
	char Emg0;
	char Emg1;
	char Emg2;
	char Emg3;
	char Emg4;
	char Emg5;
	char Emg6;
	char Emg7;
	char Pose;
	char WhichArm;
	char ArmDirection;
	bool OnPair;
	bool OnConnect;
	bool OnArmSync;
	bool OnLock;
};

class DataTransceiver
{
public:
	DataTransceiver(std::shared_ptr<std::mutex> mutex);
	~DataTransceiver();
	bool Initialize();
	void SetSendMessage(MyoInformation myoInfo);
	void GetReceiveMessage();

	bool GetIsFinish() const { return isFinish; }
	void SetIsFinish(const bool value) { isFinish = value; }

private:
	std::shared_ptr<std::thread> sendThread;
	std::shared_ptr<std::thread> receiveThread;
	std::shared_ptr<std::mutex> mutex;
	InputInformation sendData;
	std::array<char, 256> receiveData;
	bool isFinish = false;

	static const std::string LogString;
	static const std::string ErrorString;

	static void SendMessageTo(std::shared_ptr<std::mutex>& mutex, InputInformation& data, bool& isFinish);
	static void ReceiveMessageFrom(std::shared_ptr<std::mutex>& mutex,std::array<char, 256>& data, bool& isFinish);
};

