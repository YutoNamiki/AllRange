#pragma once

#include <array>
#include <vector>
#include <myo/Myo.hpp>

#pragma comment(lib, "myo64.lib")

struct MyoInformation
{
	myo::Myo* MyoPtr;
	myo::Quaternion<float> Rotation;
	myo::Vector3<float> Acceleration;
	myo::Vector3<float> Gyro;
	std::array<int8_t, 8> Emg;
	myo::Pose::Type Pose;
	bool OnPair : 1;
	bool OnConnect : 1;
	bool OnArmSync : 1;
	bool OnLock : 1;
	myo::Arm WhichArm : 2;
	myo::XDirection ArmDirection : 2;

	MyoInformation();
};

class DataCollector : public myo::DeviceListener
{
public:
	std::vector<myo::Myo*> KnownMyos;
	std::vector<MyoInformation> MyoInfos;

	DataCollector();
	~DataCollector();
	int IdentifyMyo(myo::Myo* myo);
	void PrintData(MyoInformation myo);

	virtual void onPair(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) override;
	virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) override;
	virtual void onConnect(myo::Myo* myo, uint64_t timestamp, myo::FirmwareVersion firmwareVersion) override;
	virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) override;
	virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation, myo::WarmupState warmupState) override;
	virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) override;
	virtual void onUnlock(myo::Myo* myo, uint64_t timestamp) override;
	virtual void onLock(myo::Myo* myo, uint64_t timestamp) override;
	virtual void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) override;
	virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation) override;
	virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel) override;
	virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro) override;
	virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) override;
	virtual void onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level) override;
	virtual void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) override;
	virtual void onWarmupCompleted(myo::Myo* myo, uint64_t timestamp, myo::WarmupResult warmupResult) override;

private:
	static const std::string LogString;
	static const std::string ErrorString;

};
