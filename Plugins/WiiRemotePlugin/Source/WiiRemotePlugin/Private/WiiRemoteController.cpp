#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteController.h"

UWiiRemoteController::UWiiRemoteController(const FObjectInitializer& init) : Super(init) { }

void UWiiRemoteController::SetFromWiiRemoteDevciceData(FWiiRemoteDeviceData* data)
{
	this->BatteryPercent = data->BatteryPercent;
	this->Buttons = data->Buttons;
	this->Dots.Init(FWiiRemoteDot(), 4);
	this->Dots = data->Dots;
	this->Acceleration = data->Acceleration;
	this->OrientationPitch = data->OrientationPitch;
	this->OrientationRoll = data->OrientationRoll;
	this->NunchukButtons = data->NunchukButtons;
	this->NunchukAcceleration = data->NunchukAcceleration;
	this->NunchukOrientationPitch = data->NunchukOrientationPitch;
	this->NunchukOrientationRoll = data->NunchukOrientationRoll;
	this->NunchukJoystickX = data->NunchukJoystickX;
	this->NunchukJoystickY = data->NunchukJoystickY;
	this->ClassicControllerButtons = data->ClassicControllerButtons;
	this->ClassicControllerLeftJoystickX = data->ClassicControllerLeftJoystickX;
	this->ClassicControllerLeftJoystickY = data->ClassicControllerLeftJoystickY;
	this->ClassicControllerRightJoystickX = data->ClassicControllerRightJoystickX;
	this->ClassicControllerRightJoystickY = data->ClassicControllerRightJoystickY;
	this->ClassicControllerRightTrigger = data->ClassicControllerRightTrigger;
	this->ClassicControllerLeftTrigger = data->ClassicControllerLeftTrigger;
	this->BalanceBoard = data->BalanceBoard;
	this->MotionPlusSpeed = data->MotionPlusSpeed;
	this->LED = data->LED;
}
