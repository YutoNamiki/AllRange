#pragma once

#include <array>
#include <memory>
#include <string>

struct WSAData;
struct sockaddr_in;
struct MyoInformation;

namespace std
{
	class mutex;
	class thread;
}

class UDPSocket
{
public:

	UDPSocket(std::shared_ptr<std::mutex> mutex);
	~UDPSocket();
	bool Initialize();
	void SetSendMessage(MyoInformation myoInfo);
	void GetReceiveMessage();

	bool GetIsFinish() const { return isFinish; }
	void SetIsFinish(const bool value) { isFinish = value; }

private:
	std::shared_ptr<WSAData> windowsSocketApiData;
	std::shared_ptr<std::thread> sendThread;
	std::shared_ptr<std::thread> receiveThread;
	std::shared_ptr<std::mutex> mutex;
	std::array<char, 58> sendData;
	std::array<char, 256> receiveData;
	bool isFinish = false;

	static const char* ipv4AddressIn;
	static const char* ipv4AddressFrom;
	static const unsigned short sendPort = 8001;
	static const unsigned short receivePort = 8000;
	static const std::string LogString;
	static const std::string ErrorString;

	static bool WindowSocketAPIStartup(WSAData& winSocketAPIData);
	static bool CreateSocket(std::shared_ptr<std::mutex> mutex, unsigned long long& sock);
	static void SetAddress(sockaddr_in& address, const char* ipv4Address, const unsigned short port);
	static bool BindSocket(std::shared_ptr<std::mutex> mutex, unsigned long long& sock, sockaddr_in& addressIn);
	static void CloseSocket(std::shared_ptr<std::mutex> mutex, unsigned long long& sock);
	static void WindowSocketAPICleanup();

	static void SendMessageTo(std::shared_ptr<std::mutex>& mutex, const char* ipv4Address, const unsigned short port, 
		std::array<char, 58>& data, bool& isFinish);
	static void ReceiveMessageFrom(std::shared_ptr<std::mutex>& mutex, const char* ipv4Address, const unsigned short port, 
		std::array<char, 256>& data, bool& isFinish);
};
