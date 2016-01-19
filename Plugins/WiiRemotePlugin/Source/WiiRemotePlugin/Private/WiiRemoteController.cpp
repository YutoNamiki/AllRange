#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteController.h"

UWiiRemoteController::UWiiRemoteController(const FObjectInitializer& init) : Super(init) 
{
	this->Dots.Init(FWiiRemoteDot(), 4);
	for (auto i = 0; i < 4; i++)
	{
		this->Dots[i].bVisible = false;
		this->Dots[i].X = 0.0f;
		this->Dots[i].Y = 0.0f;
	}
}

void UWiiRemoteController::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	wiiRemoteDelegate->SetLED(playerIndex, ledBits);
}

void UWiiRemoteController::SetVibrate(int32 playerIndex, bool on)
{
	wiiRemoteDelegate->SetRumble(playerIndex, on);
}

void UWiiRemoteController::SetVibrateForAsync(int32 playerIndex, int32 milliseconds)
{
	wiiRemoteDelegate->SetRumbleForAsync(playerIndex, milliseconds);
}

void UWiiRemoteController::EnableSpeaker(int32 playerIndex, bool on)
{
	wiiRemoteDelegate->EnableSpeaker(playerIndex, on);
}

void UWiiRemoteController::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequancy, int32 volume)
{
	wiiRemoteDelegate->PlaySquareWave(playerIndex, frequancy, volume);
}

void UWiiRemoteController::SetFromWiiRemoteDevciceData(FWiiRemoteDeviceData* data)
{
	this->BatteryPercent = data->BatteryPercent;
	this->Buttons = data->Buttons;
	if (data->Dots.Num() == 4)
	{
		this->Dots = TArray<FWiiRemoteDot>();
		this->Dots = data->Dots;
	}
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
