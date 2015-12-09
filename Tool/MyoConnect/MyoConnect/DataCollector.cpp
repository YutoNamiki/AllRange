#include "DataCollector.h"

DataCollector::DataCollector()
	: OnPair(false)
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

DataCollector::~DataCollector()
{

}

void DataCollector::onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
{
	OnPair = true;
}

void DataCollector::onUnpair(myo::Myo* myo, uint64_t timestamp)
{
	OnPair = false;
	Emg.fill(0);
	Acceleration = myo::Vector3<float>(0.0f, 0.0f, 0.0f);
	Gyro = myo::Vector3<float>(0.0f, 0.0f, 0.0f);
}

void DataCollector::onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion)
{
	OnConnect = true;
}

void DataCollector::onDisconnect(myo::Myo* myo, uint64_t timestamp)
{
	OnConnect = false;
}

void DataCollector::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState)
{
	OnArmSync = true;
	WhichArm = arm;
	ArmDirection = xDirection;
}

void DataCollector::onArmUnsync(myo::Myo* myo, uint64_t timestamp)
{
	OnArmSync = false;
}

void DataCollector::onUnlock(myo::Myo* myo, uint64_t timestamp)
{
	OnLock = false;
}

void DataCollector::onLock(myo::Myo* myo, uint64_t timestamp)
{
	OnLock = true;
}

void DataCollector::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
	Pose = pose;
}

void DataCollector::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation)
{
	Rotation = rotation;
}

void DataCollector::onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
{
	Acceleration = accel;
}

void DataCollector::onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
{
	Gyro = gyro;
}

void DataCollector::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi)
{

}

void DataCollector::onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level)
{

}

void DataCollector::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
{
	for (auto i = 0; i < Emg.size(); i++)
		Emg[i] = emg[i];
}

void DataCollector::onWarmupCompleted(myo::Myo* myo, uint64_t timestamp, myo::WarmupResult warmupResult)
{

}
