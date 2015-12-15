#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteManager.h"
#include "WiiRemoteDelegate.h"

#include <AllowWindowsPlatformTypes.h>
#include "wiimote.h"
#include <HideWindowsPlatformTypes.h>

UWiiRemoteManager::UWiiRemoteManager(class FObjectInitializer const& objectInitializer)
	: Super(objectInitializer)
{
	
}

UWiiRemoteManager::~UWiiRemoteManager()
{

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

void UWiiRemoteManager::OnIRChanged()
{

}

void UWiiRemoteManager::OnWiiRemoteChanged()
{

}

void UWiiRemoteManager::OnNunchukConnected()
{

}

void UWiiRemoteManager::OnNunchukButtonsChanged()
{

}

void UWiiRemoteManager::OnNunchukAccelChanged()
{

}

void UWiiRemoteManager::OnNunchukOrientationChanged()
{

}

void UWiiRemoteManager::OnNunchukJoystickChanged()
{

}

void UWiiRemoteManager::OnNunchukChanged()
{

}

void UWiiRemoteManager::OnClassicConnected()
{

}

void UWiiRemoteManager::OnClassicButtonsChanged()
{

}

void UWiiRemoteManager::OnClassicJoystickLChanged()
{

}

void UWiiRemoteManager::OnClassicJoystickRChanged()
{

}

void UWiiRemoteManager::OnClassicTriggersChanged()
{

}

void UWiiRemoteManager::OnClassicChanged()
{

}

void UWiiRemoteManager::OnBalanceConnected()
{

}

void UWiiRemoteManager::OnBalanceWeightChanged()
{

}

void UWiiRemoteManager::OnBalanceChanged()
{

}

void UWiiRemoteManager::OnMotionPlusDetected()
{

}

void UWiiRemoteManager::OnMotionPlusEnabled()
{

}

void UWiiRemoteManager::OnMotionPlusSpeedChanged()
{

}

void UWiiRemoteManager::OnMotionPlusExtensionConnected()
{

}

void UWiiRemoteManager::OnMotionPlusExtensionDisconnected()
{

}

void UWiiRemoteManager::OnMotionPlusChanged()
{

}

void UWiiRemoteManager::OnExtensionDisconnected()
{

}

void UWiiRemoteManager::OnExtensionPartiallyInserted()
{

}

void UWiiRemoteManager::OnExtensionConnected()
{

}

void UWiiRemoteManager::OnExtensionChanged()
{

}

void UWiiRemoteManager::ChangedAll()
{

}

void UWiiRemoteManager::OnStateChanged(wiimote& wiiRemote, state_change_flags changed, wiimote_state& new_state)
{
	if (changed & state_change_flags::CONNECTED)
		OnConnected(wiiRemote.UniqueID);
	if (changed & state_change_flags::CONNECTION_LOST)
		OnConnectionLost(wiiRemote.UniqueID);
	if (changed & state_change_flags::BATTERY_CHANGED)
		OnBatteryChanged(wiiRemote.UniqueID, wiiRemote.BatteryPercent);
	if (changed & state_change_flags::BATTERY_DRAINED)
		OnBatteryDrained(wiiRemote.UniqueID);
	if (changed & state_change_flags::LEDS_CHANGED)
		OnLEDsChanged(wiiRemote.UniqueID, wiiRemote.LED.Bits);
	if (changed & state_change_flags::BUTTONS_CHANGED)
	{
		FWiiRemoteButtons buttons;
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
		OnButtonsChanged(wiiRemote.UniqueID, buttons);
	}
	if (changed & state_change_flags::ACCEL_CHANGED)
		OnAccelChanged(wiiRemote.UniqueID, FVector(wiiRemote.Acceleration.X, wiiRemote.Acceleration.Y, wiiRemote.Acceleration.Z));
	if (changed & state_change_flags::ORIENTATION_CHANGED)
		OnOrientationChanged(wiiRemote.UniqueID, wiiRemote.Acceleration.Orientation.Pitch, wiiRemote.Acceleration.Orientation.Roll);

}
