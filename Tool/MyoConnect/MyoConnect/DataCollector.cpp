#include "DataCollector.h"

const std::string DataCollector::LogString = std::string("DataCollector: ");
const std::string DataCollector::ErrorString = std::string("Error: ");

MyoInformation::MyoInformation()
	: MyoPtr(nullptr)
	, OnPair(false)
	, OnConnect(false)
	, OnArmSync(false)
	, OnLock(true)
	, WhichArm(myo::Arm::armUnknown)
	, ArmDirection(myo::XDirection::xDirectionUnknown)
	, Pose(myo::Pose::unknown)
	, Rotation(0.0f, 0.0f, 0.0f, 0.0f)
	, Acceleration(0.0f, 0.0f, 0.0f)
	, Gyro(0.0f, 0.0f, 0.0f)
{
	Emg.fill(0);
}

DataCollector::DataCollector()
{
	
}

DataCollector::~DataCollector()
{

}

int DataCollector::IdentifyMyo(myo::Myo* myo)
{
	for (auto i = 0; i < KnownMyos.size(); i++)
	{
		if (KnownMyos[i] == myo)
			return i;
	}
	return 0;
}

void DataCollector::PrintData(MyoInformation myo)
{
	std::cout << LogString << "Data: " << std::endl;
	std::cout << "	OnPair: " << myo.OnPair << "	OnConnect: " << myo.OnConnect << "	OnArmSync: " << myo.OnArmSync << std::endl;
	std::cout << "	OnLock: " << myo.OnLock << "	WhichArm: " << myo.WhichArm << "	ArmDirection: " << myo.ArmDirection << "	Pose: " << myo.Pose  << std::endl;
	std::cout << "	Rotation: (x: " << myo.Rotation.x() << " y: " << myo.Rotation.y() << " z: " << myo.Rotation.z() << " w: " << myo.Rotation.w() << " )" << std::endl;
	std::cout << "	Acceleration: (x: " << myo.Acceleration.x() << " y: " << myo.Acceleration.y() << " z: " << myo.Acceleration.z() << " )" << std::endl;
	std::cout << "	Gyro: (x: " << myo.Gyro.x() << " y: " << myo.Gyro.y() << " z: " << myo.Gyro.z() << " )" << std::endl;
	std::cout << "	Emg: ";
	for (auto i = 0; i < 8; i++)
		std::cout << "[" << i << "]: " << static_cast<int>(myo.Emg[i]) << ", ";
	std::cout << std::endl;
}

void DataCollector::onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
{
	myo->setStreamEmg(myo::Myo::streamEmgEnabled);
	KnownMyos.push_back(myo);
	MyoInfos.push_back(MyoInformation());
	MyoInfos[IdentifyMyo(myo)].MyoPtr = myo;
	MyoInfos[IdentifyMyo(myo)].OnPair = true;
}

void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp)
{
	auto removeIndex = 0;
	auto index = IdentifyMyo(myo);
	for (auto i = 0; i < KnownMyos.size(); i++)
	{
		if (KnownMyos[i] == myo)
		{
			removeIndex = i;
			break;
		}
	}
	KnownMyos.erase(KnownMyos.begin() + removeIndex);
	MyoInfos[index].OnPair = false;
	MyoInfos[index].Emg.fill(0);
	MyoInfos[index].Acceleration = myo::Vector3<float>(0.0f, 0.0f, 0.0f);
	MyoInfos[index].Gyro = myo::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void DataCollector::onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
{
	MyoInfos[IdentifyMyo(myo)].OnConnect = true;
}

void DataCollector::onDisconnect(myo::Myo* myo, uint64_t timestamp)
{
	MyoInfos[IdentifyMyo(myo)].OnConnect = false;
}

void DataCollector::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState)
{
	auto index = IdentifyMyo(myo);
	MyoInfos[index].OnArmSync = true;
	MyoInfos[index].WhichArm = arm;
	MyoInfos[index].ArmDirection = xDirection;
}

void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp)
{
	MyoInfos[IdentifyMyo(myo)].OnArmSync = false;
}

void DataCollector::onUnlock(myo::Myo* myo, uint64_t timestamp)
{
	MyoInfos[IdentifyMyo(myo)].OnLock = false;
}

void DataCollector::onLock(myo::Myo* myo, uint64_t timestamp)
{
	MyoInfos[IdentifyMyo(myo)].OnLock = true;
}

void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
	MyoInfos[IdentifyMyo(myo)].Pose = pose.type();
}

void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation)
{
	MyoInfos[IdentifyMyo(myo)].Rotation = rotation;
}

void DataCollector::onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
{
	MyoInfos[IdentifyMyo(myo)].Acceleration = accel;
}

void DataCollector::onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
{
	MyoInfos[IdentifyMyo(myo)].Gyro = gyro;
}

void DataCollector::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi)
{

}

void DataCollector::onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level)
{

}

void DataCollector::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
{
	auto index = IdentifyMyo(myo);
	for (auto i = 0; i < MyoInfos[index].Emg.size(); i++)
		MyoInfos[index].Emg[i] = emg[i];
}

void DataCollector::onWarmupCompleted(myo::Myo* myo, uint64_t timestamp, myo::WarmupResult warmupResult)
{

}
