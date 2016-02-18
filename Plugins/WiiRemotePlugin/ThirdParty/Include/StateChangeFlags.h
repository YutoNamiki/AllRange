#pragma once

enum class StateChangeFlags : unsigned int
{
	NoChange = 0,
	Connected = 1 << 0,
	ConnectionLost = 1 << 1,
	BatteryChanged = 1 << 2,
	BatteryDrained = 1 << 3,
	LEDsChanged = 1 << 4,
	ButtonsChanged = 1 << 5,
	AccelerationChanged = 1 << 6,
	OrientationChanged = 1 << 7,
	IRChanged = 1 << 8,
	WiiRemoteChanged =
	StateChangeFlags::ConnectionLost |
	StateChangeFlags::BatteryChanged |
	StateChangeFlags::BatteryDrained |
	StateChangeFlags::LEDsChanged |
	StateChangeFlags::ButtonsChanged |
	StateChangeFlags::AccelerationChanged |
	StateChangeFlags::OrientationChanged |
	StateChangeFlags::IRChanged,

	NunchukConnected = 1 << 9,
	NunchukButtonsChanged = 1 << 10,
	NunchukAccelerationChanged = 1 << 11,
	NunchukOrientationChanged = 1 << 12,
	NunchukJoystickChanged = 1 << 13,
	NunchukChanged =
	StateChangeFlags::NunchukConnected |
	StateChangeFlags::NunchukButtonsChanged |
	StateChangeFlags::NunchukAccelerationChanged |
	StateChangeFlags::NunchukOrientationChanged |
	StateChangeFlags::NunchukJoystickChanged,

	ClassicConnected = 1 << 14,
	ClassicButtonsChanged = 1 << 15,
	ClassicJoystickLChanged = 1 << 16,
	ClassicJoystickRChanged = 1 << 17,
	ClassicTriggersChanged = 1 << 18,
	ClassicChanged =
	StateChangeFlags::ClassicConnected |
	StateChangeFlags::ClassicButtonsChanged |
	StateChangeFlags::ClassicJoystickLChanged |
	StateChangeFlags::ClassicJoystickRChanged |
	StateChangeFlags::ClassicTriggersChanged,

	BalanceConnected = 1 << 19,
	BalanceWeightChanged = 1 << 20,
	BalanceChanged =
	StateChangeFlags::BalanceConnected |
	StateChangeFlags::BalanceWeightChanged,

	MotionPlusDetected = 1 << 21,
	MotionPlusEnabled = 1 << 22,
	MotionPlusSpeedChanged = 1 << 23,
	MotionPlusExtensionConnected = 1 << 24,
	MotionPlusExtensionDisconnected = 1 << 25,
	MotionPlusChanged =
	StateChangeFlags::MotionPlusDetected |
	StateChangeFlags::MotionPlusEnabled |
	StateChangeFlags::MotionPlusSpeedChanged |
	StateChangeFlags::MotionPlusExtensionConnected |
	StateChangeFlags::MotionPlusExtensionDisconnected,

	ExtensionDisconnected = 1 << 26,
	ExtensionPartiallyInserted = 1 << 27,
	ExtensionConnected =
	StateChangeFlags::NunchukConnected |
	StateChangeFlags::ClassicConnected |
	StateChangeFlags::BalanceConnected |
	StateChangeFlags::MotionPlusEnabled,
	ExtensionChanged = 
	StateChangeFlags::ExtensionDisconnected |
	StateChangeFlags::NunchukChanged |
	StateChangeFlags::ClassicChanged |
	StateChangeFlags::BalanceChanged | 
	StateChangeFlags::MotionPlusChanged,

	ChangedAll = 
	StateChangeFlags::WiiRemoteChanged |
	StateChangeFlags::ExtensionChanged,

};
