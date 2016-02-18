#pragma once

#include "Acceleration.h"
#include "Joystick.h"

struct WiiNunchukCalibrationInfo
{
	Vector<unsigned char> Data;
	Vector<unsigned char> DataWithGravity;
	Vector2D<unsigned char> Minimum;
	Vector2D<unsigned char> Maximum;
	Vector2D<unsigned char> Middle;
};

struct WiiNunchuk
{
	WiiNunchukCalibrationInfo CalibrationInformation;
	Acceleration Acceleration;
	Joystick Joystick;
	bool C;
	bool Z;
};
