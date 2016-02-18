#pragma once

#include "Joystick.h"

struct WiiClassicControllerCalibrationInfo
{
	Vector2D<unsigned char> MinimumL;
	Vector2D<unsigned char> MinimumR;
	Vector2D<unsigned char> MaximumL;
	Vector2D<unsigned char> MaximumR;
	Vector2D<unsigned char> MiddleL;
	Vector2D<unsigned char> MiddleR;
	unsigned char MinimumTriggerL;
	unsigned char MaximumTriggerL;
	unsigned char MinimumTriggerR;
	unsigned char MaximumTriggerR;
};

struct WiiClassicControllerButtons
{
	enum class Mask : unsigned short
	{
		TriggerR = 0x0002,
		Plus = 0x0004,
		Home = 0x0008,
		Minus = 0x0010,
		TriggerL = 0x0020,
		Down = 0x0040,
		Right = 0x0080,
		Up = 0x0100,
		Left = 0x0200,
		ZR = 0x0400,
		X = 0x0800,
		A = 0x1000,
		Y = 0x2000,
		B = 0x4000,
		ZL = 0x8000,
		All = Mask::TriggerR | Mask::Plus | Mask::Home | Mask::Minus |
		Mask::TriggerL | Mask::Down | Mask::Right | Mask::Up | Mask::Left |
		Mask::ZR | Mask::X | Mask::A | Mask::Y | Mask::B | Mask::ZL,
	};

	unsigned short Bits;

	bool A() const { return (Bits & static_cast<unsigned short>(Mask::A)) != 0; }
	bool B() const { return (Bits & static_cast<unsigned short>(Mask::B)) != 0; }
	bool Plus() const { return (Bits & static_cast<unsigned short>(Mask::Plus)) != 0; }
	bool Minus() const { return (Bits & static_cast<unsigned short>(Mask::Minus)) != 0; }
	bool Home() const { return (Bits & static_cast<unsigned short>(Mask::Home)) != 0; }
	bool Up() const { return (Bits & static_cast<unsigned short>(Mask::Up)) != 0; }
	bool Down() const { return (Bits & static_cast<unsigned short>(Mask::Down)) != 0; }
	bool Left() const { return (Bits & static_cast<unsigned short>(Mask::Left)) != 0; }
	bool Right() const { return (Bits & static_cast<unsigned short>(Mask::Right)) != 0; }
	bool X() const { return (Bits & static_cast<unsigned short>(Mask::X)) != 0; }
	bool Y() const { return (Bits & static_cast<unsigned short>(Mask::Y)) != 0; }
	bool ZL() const { return (Bits & static_cast<unsigned short>(Mask::ZL)) != 0; }
	bool ZR() const { return (Bits & static_cast<unsigned short>(Mask::ZR)) != 0; }
	bool TriggerL() const { return (Bits & static_cast<unsigned short>(Mask::TriggerL)) != 0; }
	bool TriggerR() const { return (Bits & static_cast<unsigned short>(Mask::TriggerR)) != 0; }

};

struct WiiClassicController
{
	WiiClassicControllerCalibrationInfo CalibrationInformation;
	WiiClassicControllerButtons Buttons;
	Joystick JoystickL;
	Joystick JoystickR;
	unsigned char RawTriggerL;
	unsigned char RawTriggerR;
	float TriggerL;
	float TriggerR;
};
