#define WIN32_LEAN_AND_MEAN

#include "DataTransceiver.h"
#include "DataCollector.h"
#include <Windows.h>
#include <iostream>
#include <mutex>
#include <thread>

const std::string DataTransceiver::LogString = std::string("DataTransceiver: ");
const std::string DataTransceiver::ErrorString = std::string("Error: ");

DataTransceiver::DataTransceiver(std::shared_ptr<std::mutex> mutex)
{
	this->mutex = mutex;
	for (auto i = 0; i < sendData.size(); i++)
		sendData[i] = 0;
	for (auto i = 0; i < receiveData.size(); i++)
		receiveData[i] = 0;
}

DataTransceiver::~DataTransceiver()
{
	sendThread->join();
	receiveThread->join();
}

bool DataTransceiver::Initialize()
{
	sendThread = std::make_shared<std::thread>(&SendMessageTo, std::ref(mutex), std::ref(sendData), std::ref(isFinish));
	if (sendThread == nullptr)
	{
		std::cout << LogString << ErrorString << "sendThreadの生成に失敗しました。" << std::endl;
		return false;
	}
	receiveThread = std::make_shared<std::thread>(&ReceiveMessageFrom, std::ref(mutex), std::ref(receiveData), std::ref(isFinish));
	if (receiveThread == nullptr)
	{
		std::cout << LogString << ErrorString << "receiveThreadの生成に失敗しました。" << std::endl;
		return false;
	}
	return true;
}

void DataTransceiver::SetSendMessage(MyoInformation myoInfo)
{
	auto myoPtr = reinterpret_cast<long long>(myoInfo.MyoPtr);
	sendData[0] = static_cast<char>((myoPtr & 0x00000000000000ffLL) >> 0);
	sendData[1] = static_cast<char>((myoPtr & 0x000000000000ff00LL) >> 8);
	sendData[2] = static_cast<char>((myoPtr & 0x0000000000ff0000LL) >> 16);
	sendData[3] = static_cast<char>((myoPtr & 0x00000000ff000000LL) >> 24);
	sendData[4] = static_cast<char>((myoPtr & 0x000000ff00000000LL) >> 32);
	sendData[5] = static_cast<char>((myoPtr & 0x0000ff0000000000LL) >> 40);
	sendData[6] = static_cast<char>((myoPtr & 0x00ff000000000000LL) >> 48);
	sendData[7] = static_cast<char>((myoPtr & 0xff00000000000000LL) >> 56);

	auto rotationX = myoInfo.Rotation.x();
	auto rotationXData = *reinterpret_cast<int*>(&rotationX);
	sendData[8] = static_cast<char>((rotationXData & 0x000000ff) >> 0);
	sendData[9] = static_cast<char>((rotationXData & 0x0000ff00) >> 8);
	sendData[10] = static_cast<char>((rotationXData & 0x00ff0000) >> 16);
	sendData[11] = static_cast<char>((rotationXData & 0xff000000) >> 24);
	auto rotationY = myoInfo.Rotation.y();
	auto rotationYData = *reinterpret_cast<int*>(&rotationY);
	sendData[12] = static_cast<char>((rotationYData & 0x000000ff) >> 0);
	sendData[13] = static_cast<char>((rotationYData & 0x0000ff00) >> 8);
	sendData[14] = static_cast<char>((rotationYData & 0x00ff0000) >> 16);
	sendData[15] = static_cast<char>((rotationYData & 0xff000000) >> 24);
	auto rotationZ = myoInfo.Rotation.z();
	auto rotationZData = *reinterpret_cast<int*>(&rotationZ);
	sendData[16] = static_cast<char>((rotationZData & 0x000000ff) >> 0);
	sendData[17] = static_cast<char>((rotationZData & 0x0000ff00) >> 8);
	sendData[18] = static_cast<char>((rotationZData & 0x00ff0000) >> 16);
	sendData[19] = static_cast<char>((rotationZData & 0xff000000) >> 24);
	auto rotationW = myoInfo.Rotation.w();
	auto rotationWData = *reinterpret_cast<int*>(&rotationW);
	sendData[20] = static_cast<char>((rotationWData & 0x000000ff) >> 0);
	sendData[21] = static_cast<char>((rotationWData & 0x0000ff00) >> 8);
	sendData[22] = static_cast<char>((rotationWData & 0x00ff0000) >> 16);
	sendData[23] = static_cast<char>((rotationWData & 0xff000000) >> 24);

	auto accelerationX = myoInfo.Acceleration.x();
	auto accelerationXData = *reinterpret_cast<int*>(&accelerationX);
	sendData[24] = static_cast<char>((accelerationXData & 0x000000ff) >> 0);
	sendData[25] = static_cast<char>((accelerationXData & 0x0000ff00) >> 8);
	sendData[26] = static_cast<char>((accelerationXData & 0x00ff0000) >> 16);
	sendData[27] = static_cast<char>((accelerationXData & 0xff000000) >> 24);
	auto accelerationY = myoInfo.Acceleration.y();
	auto accelerationYData = *reinterpret_cast<int*>(&accelerationY);
	sendData[28] = static_cast<char>((accelerationYData & 0x000000ff) >> 0);
	sendData[29] = static_cast<char>((accelerationYData & 0x0000ff00) >> 8);
	sendData[30] = static_cast<char>((accelerationYData & 0x00ff0000) >> 16);
	sendData[31] = static_cast<char>((accelerationYData & 0xff000000) >> 24);
	auto accelerationZ = myoInfo.Acceleration.z();
	auto accelerationZData = *reinterpret_cast<int*>(&accelerationZ);
	sendData[32] = static_cast<char>((accelerationZData & 0x000000ff) >> 0);
	sendData[33] = static_cast<char>((accelerationZData & 0x0000ff00) >> 8);
	sendData[34] = static_cast<char>((accelerationZData & 0x00ff0000) >> 16);
	sendData[35] = static_cast<char>((accelerationZData & 0xff000000) >> 24);

	auto gyroX = myoInfo.Gyro.x();
	auto gyroXData = *reinterpret_cast<int*>(&gyroX);
	sendData[36] = static_cast<char>((gyroXData & 0x000000ff) >> 0);
	sendData[37] = static_cast<char>((gyroXData & 0x0000ff00) >> 8);
	sendData[38] = static_cast<char>((gyroXData & 0x00ff0000) >> 16);
	sendData[39] = static_cast<char>((gyroXData & 0xff000000) >> 24);
	auto gyroY = myoInfo.Gyro.y();
	auto gyroYData = *reinterpret_cast<int*>(&gyroY);
	sendData[40] = static_cast<char>((gyroYData & 0x000000ff) >> 0);
	sendData[41] = static_cast<char>((gyroYData & 0x0000ff00) >> 8);
	sendData[42] = static_cast<char>((gyroYData & 0x00ff0000) >> 16);
	sendData[43] = static_cast<char>((gyroYData & 0xff000000) >> 24);
	auto gyroZ = myoInfo.Gyro.z();
	auto gyroZData = *reinterpret_cast<int*>(&gyroZ);
	sendData[44] = static_cast<char>((gyroZData & 0x000000ff) >> 0);
	sendData[45] = static_cast<char>((gyroZData & 0x0000ff00) >> 8);
	sendData[46] = static_cast<char>((gyroZData & 0x00ff0000) >> 16);
	sendData[47] = static_cast<char>((gyroZData & 0xff000000) >> 24);

	auto emg = myoInfo.Emg;
	sendData[48] = static_cast<char>(emg[0]);
	sendData[49] = static_cast<char>(emg[1]);
	sendData[50] = static_cast<char>(emg[2]);
	sendData[51] = static_cast<char>(emg[3]);
	sendData[52] = static_cast<char>(emg[4]);
	sendData[53] = static_cast<char>(emg[5]);
	sendData[54] = static_cast<char>(emg[6]);
	sendData[55] = static_cast<char>(emg[7]);

	char pose = (myoInfo.Pose == myo::Pose::Type::unknown) ? 6 : myoInfo.Pose;
	char whichArm = myoInfo.WhichArm;
	char armDirection = myoInfo.ArmDirection;
	sendData[56] = ((pose & 0b0111) << 5) | ((whichArm & 0b0011) << 2) | ((armDirection & 0b0011) << 0);
	char onPair = myoInfo.OnPair;
	char onConnect = myoInfo.OnConnect;
	char onArmSync = myoInfo.OnArmSync;
	char onLock = myoInfo.OnLock;
	sendData[57] = ((onPair & 0b0001) << 3) | ((onConnect & 0b0001) << 2) | ((onArmSync & 0b0001) << 1) | ((onLock & 0b0001) << 0);
}

void DataTransceiver::GetReceiveMessage()
{

}

void DataTransceiver::SendMessageTo(std::shared_ptr<std::mutex>& mutex, std::array<char, 58>& data, bool& isFinish)
{
	const auto fileMapName = "MyoSendData";
	char* pString = nullptr;
	HANDLE handleFileMemory = nullptr;

	while (!isFinish)
	{
		handleFileMemory = CreateFileMappingA(reinterpret_cast<HANDLE>(-1), nullptr, PAGE_READWRITE, 0, static_cast<DWORD>(data.size() + 1), fileMapName);
		if (handleFileMemory == NULL)
		{
			mutex->lock();
			std::cout << LogString << ErrorString << "MyoSendDataのファイルマッピングオブジェクトの作成に失敗しました。" << std::endl;
			mutex->unlock();
			continue;
		}
		pString = static_cast<char*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pString == nullptr)
		{
			mutex->lock();
			std::cout << LogString << ErrorString << "マッピングに失敗しました。" << std::endl;
			mutex->unlock();
			continue;
		}
		auto message = data.data();
		auto size = data.size();
		memcpy_s(pString, size, message, size);
		if (UnmapViewOfFile(pString) == 0)
		{
			mutex->lock();
			std::cout << LogString << ErrorString << "アンマッピングに失敗しました。" << std::endl;
			mutex->unlock();
		}
		pString = nullptr;
	}
	CloseHandle(handleFileMemory);
	handleFileMemory = nullptr;
	mutex->lock();
	std::cout << LogString << "MyoSendDataの送信を終了します。" << std::endl;
	mutex->unlock();
}

void DataTransceiver::ReceiveMessageFrom(std::shared_ptr<std::mutex>& mutex, std::array<char, 256>& data, bool& isFinish)
{
	const auto fileMapName = "MyoReceiveData";
	char* pString = nullptr;
	HANDLE handleFileMemory = nullptr;

	while (!isFinish)
	{
		handleFileMemory = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, fileMapName);
		pString = static_cast<char*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pString == nullptr)
			continue;
		mutex->lock();
		std::cout << LogString << "Unreal Engine からデータを受信しました。" << std::endl;
		mutex->unlock();
		if (UnmapViewOfFile(pString) == 0)
		{
			mutex->lock();
			std::cout << LogString << ErrorString << "アンマッピングに失敗しました。" << std::endl;
			mutex->unlock();
		}
		CloseHandle(handleFileMemory);
		handleFileMemory = nullptr;
		pString = nullptr;
	}
	if (handleFileMemory != nullptr)
	{
		CloseHandle(handleFileMemory);
		handleFileMemory = nullptr;
	}
	mutex->lock();
	std::cout << LogString << "MyoReceiveDataの受信を終了します。" << std::endl;
	mutex->unlock();
}
