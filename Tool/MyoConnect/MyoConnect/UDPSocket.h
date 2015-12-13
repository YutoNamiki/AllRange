#pragma once

#include <array>
#include <memory>
#include <string>

struct WSAData;
struct sockaddr_in;
struct MyoInformation;

namespace std
{
	class thread;
}

class UDPSocket
{
public:

	UDPSocket();
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
	static bool CreateSocket(unsigned long long& sock);
	static void SetAddress(sockaddr_in& address, const char* ipv4Address, const unsigned short port);
	static bool BindSocket(unsigned long long& sock, sockaddr_in& addressIn);
	static void CloseSocket(unsigned long long& sock);
	static void WindowSocketAPICleanup();

	static void SendMessageTo(const char* ipv4Address, const unsigned short port, std::array<char, 58>& data, bool& isFinish);
	static void ReceiveMessageFrom(const char* ipv4Address, const unsigned short port, std::array<char, 256>& data, bool& isFinish);
};
