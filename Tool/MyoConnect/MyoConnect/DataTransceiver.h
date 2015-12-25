#pragma once

#include <array>
#include <memory>

struct MyoInformation;

namespace std
{
	class mutex;
	class thread;
}

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
	std::array<char, 58> sendData;
	std::array<char, 256> receiveData;
	bool isFinish = false;

	static const std::string LogString;
	static const std::string ErrorString;

	static void SendMessageTo(std::shared_ptr<std::mutex>& mutex, std::array<char, 58>& data, bool& isFinish);
	static void ReceiveMessageFrom(std::shared_ptr<std::mutex>& mutex,std::array<char, 256>& data, bool& isFinish);
};

