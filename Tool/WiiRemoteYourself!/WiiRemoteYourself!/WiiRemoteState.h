#pragma once

#include "IR.h"
#include "Speaker.h"
#include "WiiNunchuk.h"
#include "WiiClassicController.h"
#include "WiiBalanceBoard.h"
#include "WiiMotionPlus.h"

enum class ExtensionType
{
	None,
	Nunchuk,
	Classic,
	GH3GhwtGuitar,
	GhwtDrums,
	BalanceBoard,
	MotionPlus,
	WiiRemotePlus,
	PartiallyInserted,
};

struct WiiRemoteCalibrationInfo
{
	Vector<unsigned char> Data;
	Vector<unsigned char> DataWithGravity;
};

struct WiiRemoteButtons
{
	enum class Mask : unsigned short
	{
		Left = 0x0001,
		Right = 0x0002,
		Down = 0x0004,
		Up = 0x0008,
		Plus = 0x0010,
		Two = 0x0100,
		One = 0x0200,
		B = 0x0400,
		A = 0x0800,
		Minus = 0x1000,
		Home = 0x8000,
		All = Left | Right | Down | Up | Plus | Two | One | A | B | Minus | Home,
	};

	unsigned short Bits;

	bool A() const { return (Bits & static_cast<unsigned short>(Mask::A)) != 0; }
	bool B() const { return (Bits & static_cast<unsigned short>(Mask::B)) != 0; }
	bool Plus() const { return (Bits & static_cast<unsigned short>(Mask::Plus)) != 0; }
	bool Home() const { return (Bits & static_cast<unsigned short>(Mask::Home)) != 0; }
	bool Minus() const { return (Bits & static_cast<unsigned short>(Mask::Minus)) != 0; }
	bool One() const { return (Bits & static_cast<unsigned short>(Mask::One)) != 0; }
	bool Two() const { return (Bits & static_cast<unsigned short>(Mask::Two)) != 0; }
	bool Up() const { return (Bits & static_cast<unsigned short>(Mask::Up)) != 0; }
	bool Down() const { return (Bits & static_cast<unsigned short>(Mask::Down)) != 0; }
	bool Left() const { return (Bits & static_cast<unsigned short>(Mask::Left)) != 0; }
	bool Right() const { return (Bits & static_cast<unsigned short>(Mask::Right)) != 0; }
};

struct LED
{
	unsigned char Bits;

	bool Lit(const unsigned char index) const { return (index >= 4) ? false : ((Bits & (1 << index)) != 0); }
};

struct WiiRemoteState
{
	friend class WiiRemote;

	WiiRemoteCalibrationInfo CalibrationInformation;
	WiiRemoteButtons Buttons;
	Acceleration Acceleration;
	IR IR;
	LED LED;
	unsigned char BatteryRaw;
	unsigned char BatteryPercent;
	bool bBatteryDrained;
	bool bRumble;
	bool bExtension;
	Speaker Speaker;
	ExtensionType ExtensionType;
	WiiNunchuk Nunchuk;
	WiiClassicController ClassicController;
	WiiBalanceBoard BalanceBoard;
	WiiMotionPlus MotionPlus;

protected:
	unsigned int wiiRemoteNearGravityUpdates;
	unsigned int wiiNunchukNearGravityUpdates;

	void Clear(const bool includingDeadzones);
};

struct WiiRemoteStateEvent
{
	unsigned long TimeMs;
	WiiRemoteState State;
};
