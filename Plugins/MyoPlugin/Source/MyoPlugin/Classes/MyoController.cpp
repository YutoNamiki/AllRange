#include "MyoPluginPrivatePCH.h"
#include "MyoController.h"

UMyoController::UMyoController(const FObjectInitializer &init) : Super(init)
{

}

bool UMyoController::isOnLeftArm()
{
	return Arm == MyoArm::Left;
}

bool UMyoController::isOnRightArm()
{
	return Arm == MyoArm::Right;
}

void UMyoController::VibrateDevice(MyoVibrationType type)
{
	myoDelegate->MyoVibrateDevice(this->MyoId, type);
}

void UMyoController::CalibrateArmOrientation(FRotator direction)
{
	myoDelegate->MyoCalibrateArmOrientation(this->MyoId, direction);
}

void UMyoController::Unlock(MyoUnlockType type)
{
	myoDelegate->MyoUnlockMyo(this->MyoId, type);
}

void UMyoController::Lock()
{
	myoDelegate->MyoLockMyo(this->MyoId);
}

void UMyoController::SetStreamEmg(MyoStreamEmgType streamType)
{
	myoDelegate->MyoSetStreamEmg(this->MyoId, streamType);
}

void UMyoController::SetFromMyoDeviceData(FMyoDeviceData* data)
{
	this->Pose = data->Pose;
	this->Acceleration = data->Acceleration;
	this->Orientation = data->Orientation;
	this->Gyro = data->Gyro;
	this->Arm = data->Arm;
	this->XDirection = data->XDirection;
	this->ArmAcceleration = data->ArmAcceleration;
	this->ArmOrientation = data->ArmOrientation;
	this->ArmGyro = data->ArmGyro;
	this->ArmSpaceCorrection = data->ArmSpaceCorrection;
	this->BodySpaceNullAcceleration = data->BodySpaceNullAcceleration;
}
