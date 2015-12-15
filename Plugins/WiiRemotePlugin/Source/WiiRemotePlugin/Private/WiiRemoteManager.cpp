#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteManager.h"
#include "WiiRemoteDelegate.h"

#include "AllowWindowsPlatformTypes.h"
#include "wiimote.h"
#include "HideWindowsPlatformTypes.h"

UWiiRemoteManager::UWiiRemoteManager(class FObjectInitializer const& objectInitializer)
	: Super(objectInitializer)
	, WiiRemoteDelegate(nullptr)
{
	
}

UWiiRemoteManager::~UWiiRemoteManager()
{
	ShutDown();
}

bool UWiiRemoteManager::Startup()
{
	return false;
}

void UWiiRemoteManager::ShutDown()
{

}

void UWiiRemoteManager::Reset()
{

}

void UWiiRemoteManager::Tick(float deltaTime)
{
	
}

int32 UWiiRemoteManager::IdentifyWiiRemote(uint64 wiiRemoteId)
{
	return 0;
}

uint64 UWiiRemoteManager::LastValidWiiRemote()
{
	return 0;
}

bool UWiiRemoteManager::IsWiiRemoteValidForInputMapping(uint64 wiiRemoteId)
{
	return false;
}

int32 UWiiRemoteManager::WiiRemoteIndexForWiiRemote(uint64 wiiRemoteId)
{
	return 0;
}

void UWiiRemoteManager::OnConnected(uint64 id)
{

}

void UWiiRemoteManager::OnConnectionLost(uint64 id)
{

}

void UWiiRemoteManager::OnBatteryChanged(uint64 id, uint8 batteryPercent)
{

}

void UWiiRemoteManager::OnBatteryDrained(uint64 id)
{

}

void UWiiRemoteManager::OnLEDsChanged(uint64 id, uint8 ledBits)
{

}

void UWiiRemoteManager::OnButtonsChanged(uint64 id, FWiiRemoteButtons buttons)
{

}

void UWiiRemoteManager::OnAccelChanged(uint64 id, FVector accel)
{

}

void UWiiRemoteManager::OnOrientationChanged(uint64 id, float pitch, float roll)
{

}

void UWiiRemoteManager::OnIRChanged(uint64 id, TArray<FWiiRemoteDot> dots)
{

}

void UWiiRemoteManager::OnNunchukConnected(uint64 id)
{

}

void UWiiRemoteManager::OnNunchukButtonsChanged(uint64 id, FWiiRemoteNunchukButtons buttons)
{

}

void UWiiRemoteManager::OnNunchukAccelChanged(uint64 id, FVector accel)
{

}

void UWiiRemoteManager::OnNunchukOrientationChanged(uint64 id, float pitch, float roll)
{

}

void UWiiRemoteManager::OnNunchukJoystickChanged(uint64 id, float x, float y)
{

}

void UWiiRemoteManager::OnClassicConnected(uint64 id)
{

}

void UWiiRemoteManager::OnClassicButtonsChanged(uint64 id, FWiiRemoteClassicControllerButtons buttons)
{

}

void UWiiRemoteManager::OnClassicJoystickLChanged(uint64 id, float x, float y)
{

}

void UWiiRemoteManager::OnClassicJoystickRChanged(uint64 id, float x, float y)
{

}

void UWiiRemoteManager::OnClassicTriggersChanged(uint64 id, float left, float right)
{

}

void UWiiRemoteManager::OnBalanceConnected(uint64 id)
{

}

void UWiiRemoteManager::OnBalanceWeightChanged(uint64 id, FWiiRemoteBalanceBoard balanceBoard)
{

}

void UWiiRemoteManager::OnMotionPlusDetected(uint64 id)
{

}

void UWiiRemoteManager::OnMotionPlusEnabled(uint64 id)
{

}

void UWiiRemoteManager::OnMotionPlusSpeedChanged(uint64 id, FRotator speed)
{

}

void UWiiRemoteManager::OnMotionPlusExtensionConnected(uint64 id)
{

}

void UWiiRemoteManager::OnMotionPlusExtensionDisconnected(uint64 id)
{

}

void UWiiRemoteManager::OnExtensionDisconnected(uint64 id)
{
	
}

void UWiiRemoteManager::OnStateChanged(wiimote& wiiRemote, state_change_flags changed, wiimote_state& new_state)
{
	if (changed & state_change_flags::CONNECTED)
	{
		if (new_state.ExtensionType != wiimote::BALANCE_BOARD)
		{
			if (new_state.bExtension)
				wiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
			else
				wiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
		}
		OnConnected(wiiRemote.UniqueID);
	}
	if (changed & state_change_flags::CONNECTION_LOST)
		OnConnectionLost(wiiRemote.UniqueID);
	if (changed & state_change_flags::CHANGED_ALL)
		wiiRemote.RefreshState();
	if (changed & state_change_flags::BATTERY_CHANGED)
		OnBatteryChanged(wiiRemote.UniqueID, wiiRemote.BatteryPercent);
	if (changed & state_change_flags::BATTERY_DRAINED)
		OnBatteryDrained(wiiRemote.UniqueID);
	if (changed & state_change_flags::LEDS_CHANGED)
		OnLEDsChanged(wiiRemote.UniqueID, wiiRemote.LED.Bits);
	if (changed & state_change_flags::BUTTONS_CHANGED)
	{
		FWiiRemoteButtons buttons;
		SetWiiRemoteButtons(wiiRemote, buttons);
		OnButtonsChanged(wiiRemote.UniqueID, buttons);
	}
	if (changed & state_change_flags::ACCEL_CHANGED)
		OnAccelChanged(wiiRemote.UniqueID, FVector(wiiRemote.Acceleration.X, wiiRemote.Acceleration.Y, wiiRemote.Acceleration.Z));
	if (changed & state_change_flags::ORIENTATION_CHANGED)
		OnOrientationChanged(wiiRemote.UniqueID, wiiRemote.Acceleration.Orientation.Pitch, wiiRemote.Acceleration.Orientation.Roll);
	if (changed & state_change_flags::IR_CHANGED)
	{
		TArray<FWiiRemoteDot> dots;
		SetWiiRemoteDots(wiiRemote, dots);
		OnIRChanged(wiiRemote.UniqueID, dots);
	}
	if (changed & state_change_flags::NUNCHUK_CONNECTED)
	{
		wiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_EXT);
		OnNunchukConnected(wiiRemote.UniqueID);
	}
	if (changed & state_change_flags::NUNCHUK_BUTTONS_CHANGED)
	{
		FWiiRemoteNunchukButtons buttons;
		SetWiiRemoteNunchukButtons(wiiRemote, buttons);
		OnNunchukButtonsChanged(wiiRemote.UniqueID, buttons);
	}
	if (changed & state_change_flags::NUNCHUK_ACCEL_CHANGED)
		OnNunchukAccelChanged(wiiRemote.UniqueID, FVector(wiiRemote.Nunchuk.Acceleration.X, wiiRemote.Nunchuk.Acceleration.Y, wiiRemote.Nunchuk.Acceleration.Z));
	if (changed & state_change_flags::NUNCHUK_ORIENTATION_CHANGED)
		OnNunchukOrientationChanged(wiiRemote.UniqueID, wiiRemote.Nunchuk.Acceleration.Orientation.Pitch, wiiRemote.Nunchuk.Acceleration.Orientation.Roll);
	if (changed & state_change_flags::NUNCHUK_JOYSTICK_CHANGED)
		OnNunchukJoystickChanged(wiiRemote.UniqueID, wiiRemote.Nunchuk.Joystick.X, wiiRemote.Nunchuk.Joystick.Y);
	if (changed & state_change_flags::CLASSIC_CONNECTED)
	{
		wiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_EXT);
		OnClassicConnected(wiiRemote.UniqueID);
	}
	if (changed & state_change_flags::CLASSIC_BUTTONS_CHANGED)
	{
		FWiiRemoteClassicControllerButtons buttons;
		SetWiiRemoteClassicControllerButtons(wiiRemote, buttons);
		OnClassicButtonsChanged(wiiRemote.UniqueID, buttons);
	}
	if (changed & state_change_flags::CLASSIC_JOYSTICK_L_CHANGED)
		OnClassicJoystickLChanged(wiiRemote.UniqueID, wiiRemote.ClassicController.JoystickL.X, wiiRemote.ClassicController.JoystickL.Y);
	if (changed & state_change_flags::CLASSIC_JOYSTICK_R_CHANGED)
		OnClassicJoystickRChanged(wiiRemote.UniqueID, wiiRemote.ClassicController.JoystickR.X, wiiRemote.ClassicController.JoystickR.Y);
	if (changed & state_change_flags::CLASSIC_TRIGGERS_CHANGED)
		OnClassicTriggersChanged(wiiRemote.UniqueID, wiiRemote.ClassicController.TriggerL, wiiRemote.ClassicController.TriggerR);
	if (changed & state_change_flags::BALANCE_CONNECTED)
	{
		wiiRemote.SetReportType(wiimote::IN_BUTTONS_BALANCE_BOARD);
		OnBalanceConnected(wiiRemote.UniqueID);
	}
	if (changed & state_change_flags::BALANCE_WEIGHT_CHANGED)
	{
		FWiiRemoteBalanceBoard balanceBoard;
		SetWiiRemoteBalanceBoard(wiiRemote, balanceBoard);
		OnBalanceWeightChanged(wiiRemote.UniqueID, balanceBoard);
	}
	if (changed & state_change_flags::MOTIONPLUS_DETECTED)
	{
		wiiRemote.EnableMotionPlus();
		OnMotionPlusDetected(wiiRemote.UniqueID);
	}
	if (changed & state_change_flags::MOTIONPLUS_ENABLED)
		OnMotionPlusEnabled(wiiRemote.UniqueID);
	if (changed & state_change_flags::MOTIONPLUS_SPEED_CHANGED)
		OnMotionPlusSpeedChanged(wiiRemote.UniqueID, FRotator(wiiRemote.MotionPlus.Speed.Pitch, wiiRemote.MotionPlus.Speed.Yaw, wiiRemote.MotionPlus.Speed.Roll));
	if (changed & state_change_flags::MOTIONPLUS_EXTENSION_CONNECTED)
	{
		if (wiiRemote.MotionPlusEnabled())
			wiiRemote.EnableMotionPlus();
		OnMotionPlusExtensionConnected(wiiRemote.UniqueID);
	}
	if (changed & state_change_flags::MOTIONPLUS_EXTENSION_DISCONNECTED)
	{
		if (wiiRemote.MotionPlusConnected())
			wiiRemote.EnableMotionPlus();
		OnMotionPlusExtensionDisconnected(wiiRemote.UniqueID);
	}
	if (changed & state_change_flags::EXTENSION_DISCONNECTED)
	{
		wiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
		OnExtensionDisconnected(wiiRemote.UniqueID);
	}
}

void UWiiRemoteManager::SetWiiRemoteButtons(wiimote& wiiRemote, FWiiRemoteButtons& buttons)
{
	buttons.IsPushA = wiiRemote.Button.A();
	buttons.IsPushB = wiiRemote.Button.B();
	buttons.IsPushUp = wiiRemote.Button.Up();
	buttons.IsPushDown = wiiRemote.Button.Down();
	buttons.IsPushRight = wiiRemote.Button.Right();
	buttons.IsPushLeft = wiiRemote.Button.Left();
	buttons.IsPushOne = wiiRemote.Button.One();
	buttons.IsPushTwo = wiiRemote.Button.Two();
	buttons.IsPushPlus = wiiRemote.Button.Plus();
	buttons.IsPushMinus = wiiRemote.Button.Minus();
	buttons.IsPushHome = wiiRemote.Button.Home();
}

void UWiiRemoteManager::SetWiiRemoteDots(wiimote& wiiRemote, TArray<FWiiRemoteDot>& dots)
{
	dots.Init(FWiiRemoteDot(), 4);
	for (auto index = 0; index < dots.Num(); index++)
	{
		dots[index].bVisible = wiiRemote.IR.Dot[index].bVisible;
		dots[index].X = wiiRemote.IR.Dot[index].X;
		dots[index].Y = wiiRemote.IR.Dot[index].Y;
	}
}

void UWiiRemoteManager::SetWiiRemoteNunchukButtons(wiimote& wiiRemote, FWiiRemoteNunchukButtons& buttons)
{
	buttons.IsPushC = wiiRemote.Nunchuk.C;
	buttons.IsPushZ = wiiRemote.Nunchuk.Z;
}

void UWiiRemoteManager::SetWiiRemoteClassicControllerButtons(wiimote& wiiRemote, FWiiRemoteClassicControllerButtons& buttons)
{
	buttons.IsPushA = wiiRemote.ClassicController.Button.A();
	buttons.IsPushB = wiiRemote.ClassicController.Button.B();
	buttons.IsPushX = wiiRemote.ClassicController.Button.X();
	buttons.IsPushY = wiiRemote.ClassicController.Button.Y();
	buttons.IsPushUp = wiiRemote.ClassicController.Button.Up();
	buttons.IsPushDown = wiiRemote.ClassicController.Button.Down();
	buttons.IsPushRight = wiiRemote.ClassicController.Button.Right();
	buttons.IsPushLeft = wiiRemote.ClassicController.Button.Left();
	buttons.IsPushPlus = wiiRemote.ClassicController.Button.Plus();
	buttons.IsPushMinus = wiiRemote.ClassicController.Button.Minus();
	buttons.IsPushHome = wiiRemote.ClassicController.Button.Home();
	buttons.IsPushZL = wiiRemote.ClassicController.Button.ZL();
	buttons.IsPushZR = wiiRemote.ClassicController.Button.ZR();
	buttons.IsPushTriggerL = wiiRemote.ClassicController.Button.TriggerL();
	buttons.IsPushTriggerR = wiiRemote.ClassicController.Button.TriggerR();
}

void UWiiRemoteManager::SetWiiRemoteBalanceBoard(wiimote& wiiRemote, FWiiRemoteBalanceBoard& balanceBoard)
{
	balanceBoard.AtRestKilograms.BottomL = wiiRemote.BalanceBoard.AtRestKg.BottomL;
	balanceBoard.AtRestKilograms.BottomR = wiiRemote.BalanceBoard.AtRestKg.BottomR;
	balanceBoard.AtRestKilograms.TopL = wiiRemote.BalanceBoard.AtRestKg.TopL;
	balanceBoard.AtRestKilograms.TopR = wiiRemote.BalanceBoard.AtRestKg.TopR;
	balanceBoard.AtRestKilograms.Total = wiiRemote.BalanceBoard.AtRestKg.Total;
	balanceBoard.Kilograms.BottomL = wiiRemote.BalanceBoard.Kg.BottomL;
	balanceBoard.Kilograms.BottomR = wiiRemote.BalanceBoard.Kg.BottomR;
	balanceBoard.Kilograms.TopL = wiiRemote.BalanceBoard.Kg.TopL;
	balanceBoard.Kilograms.TopR = wiiRemote.BalanceBoard.Kg.TopR;
	balanceBoard.Kilograms.Total = wiiRemote.BalanceBoard.Kg.Total;
	balanceBoard.Pounds.BottomL = wiiRemote.BalanceBoard.Lb.BottomL;
	balanceBoard.Pounds.BottomR = wiiRemote.BalanceBoard.Lb.BottomR;
	balanceBoard.Pounds.TopL = wiiRemote.BalanceBoard.Lb.TopL;
	balanceBoard.Pounds.TopR = wiiRemote.BalanceBoard.Lb.TopR;
	balanceBoard.Pounds.Total = wiiRemote.BalanceBoard.Lb.Total;
}
