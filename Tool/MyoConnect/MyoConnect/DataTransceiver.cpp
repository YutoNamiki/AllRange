#define WIN32_LEAN_AND_MEAN

#include "DataTransceiver.h"
#include "DataCollector.h"
#include <Windows.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <myo/Myo.hpp>

const std::string DataTransceiver::LogString = std::string("DataTransceiver: ");
const std::string DataTransceiver::ErrorString = std::string("Error: ");

DataTransceiver::DataTransceiver(std::shared_ptr<std::mutex> mutex)
{
	this->mutex = mutex;
	receiveData.Ptr = nullptr;
	receiveData.IsLockOrder = false;
	receiveData.LockingPolicy = -1;
	receiveData.StreamEmgType = -1;
	receiveData.UnlockType = -1;
	receiveData.Vibration = -1;
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
	sendData.Ptr = myoInfo.MyoPtr;
	sendData.RotationX = myoInfo.Rotation.x();
	sendData.RotationY = myoInfo.Rotation.y();
	sendData.RotationZ = myoInfo.Rotation.z();
	sendData.RotationW = myoInfo.Rotation.w();
	sendData.AccelerationX = myoInfo.Acceleration.x();
	sendData.AccelerationY = myoInfo.Acceleration.y();
	sendData.AccelerationZ = myoInfo.Acceleration.z();
	sendData.GyroX = myoInfo.Gyro.x();
	sendData.GyroY = myoInfo.Gyro.y();
	sendData.GyroZ = myoInfo.Gyro.z();
	sendData.Emg0 = myoInfo.Emg[0];
	sendData.Emg1 = myoInfo.Emg[1];
	sendData.Emg2 = myoInfo.Emg[2];
	sendData.Emg3 = myoInfo.Emg[3];
	sendData.Emg4 = myoInfo.Emg[4];
	sendData.Emg5 = myoInfo.Emg[5];
	sendData.Emg6 = myoInfo.Emg[6];
	sendData.Emg7 = myoInfo.Emg[7];
	sendData.Pose = myoInfo.Pose;
	sendData.WhichArm = myoInfo.WhichArm;
	sendData.ArmDirection = myoInfo.ArmDirection;
	sendData.OnPair = myoInfo.OnPair;
	sendData.OnConnect = myoInfo.OnConnect;
	sendData.OnArmSync = myoInfo.OnArmSync;
	sendData.OnLock = myoInfo.OnLock;
}

void DataTransceiver::GetReceiveMessage(std::shared_ptr<myo::Hub>& hub, std::vector<myo::Myo*>& myos)
{
	if (receiveData.LockingPolicy != -1)
		hub->setLockingPolicy(static_cast<myo::Hub::LockingPolicy>(receiveData.LockingPolicy));
	if (receiveData.Ptr != nullptr)
	{
		auto index = MyoIndexForMyo(receiveData.Ptr, myos);
		if (index == -1)
		{
			receiveData.Ptr = nullptr;
			receiveData.IsLockOrder = false;
			receiveData.LockingPolicy = -1;
			receiveData.StreamEmgType = -1;
			receiveData.UnlockType = -1;
			receiveData.Vibration = -1;
			return;
		}
		if (receiveData.IsLockOrder)
			myos[index]->lock();
		if (receiveData.StreamEmgType != -1)
			myos[index]->setStreamEmg(static_cast<myo::Myo::StreamEmgType>(receiveData.StreamEmgType));
		if (receiveData.UnlockType != -1)
			myos[index]->unlock(static_cast<myo::Myo::UnlockType>(receiveData.UnlockType));
		if (receiveData.Vibration != -1)
			myos[index]->vibrate(static_cast<myo::Myo::VibrationType>(receiveData.Vibration));
	}
	receiveData.Ptr = nullptr;
	receiveData.IsLockOrder = false;
	receiveData.LockingPolicy = -1;
	receiveData.StreamEmgType = -1;
	receiveData.UnlockType = -1;
	receiveData.Vibration = -1;
}

void DataTransceiver::SendMessageTo(std::shared_ptr<std::mutex>& mutex, InputInformation& data, bool& isFinish)
{
	const auto fileMapName = "MyoSendData";
	auto size = sizeof(InputInformation);
	InputInformation* pData = nullptr;
	HANDLE handleFileMemory = nullptr;

	handleFileMemory = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, static_cast<DWORD>(size), fileMapName);
	if (handleFileMemory == NULL)
	{
		mutex->lock();
		std::cout << LogString << ErrorString << "MyoSendDataのファイルマッピングオブジェクトの作成に失敗しました。" << std::endl;
		mutex->unlock();
		return;
	}
	while (!isFinish)
	{
		pData = static_cast<InputInformation*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pData == nullptr)
		{
			mutex->lock();
			std::cout << LogString << ErrorString << "マッピングに失敗しました。" << std::endl;
			mutex->unlock();
			continue;
		}
		auto message = &data;
		memcpy_s(pData, size, message, size);
		if (UnmapViewOfFile(pData) == 0)
		{
			mutex->lock();
			std::cout << LogString << ErrorString << "アンマッピングに失敗しました。" << std::endl;
			mutex->unlock();
		}
	}
	CloseHandle(handleFileMemory);
	handleFileMemory = nullptr;
	mutex->lock();
	std::cout << LogString << "MyoSendDataの送信を終了します。" << std::endl;
	mutex->unlock();
}

void DataTransceiver::ReceiveMessageFrom(std::shared_ptr<std::mutex>& mutex, OutputInformation& data, bool& isFinish)
{
	const auto fileMapName = "MyoReceiveData";
	auto size = sizeof(OutputInformation);
	auto isConnect = false;
	OutputInformation* pData = nullptr;
	HANDLE handleFileMemory = nullptr;

	while (!isFinish)
	{
		handleFileMemory = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, fileMapName);
		pData = static_cast<OutputInformation*>(MapViewOfFile(handleFileMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0));
		if (pData == nullptr)
		{
			isConnect = false;
			continue;
		}
		if (!isConnect)
		{
			mutex->lock();
			std::cout << LogString << "Unreal Engine からデータを受信しました。" << std::endl;
			mutex->unlock();
			isConnect = true;
		}
		memcpy_s(&data, size, pData, size);
		if (UnmapViewOfFile(pData) == 0)
		{
			mutex->lock();
			std::cout << LogString << ErrorString << "アンマッピングに失敗しました。" << std::endl;
			mutex->unlock();
		}
		CloseHandle(handleFileMemory);
		handleFileMemory = nullptr;
		pData = nullptr;
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

int DataTransceiver::MyoIndexForMyo(void* myoPtr, std::vector<myo::Myo*>& myos)
{
	for (auto i = 0; i < myos.size(); i++)
	{
		if (myos[i] == myoPtr)
			return i;
	}
	return -1;
}
