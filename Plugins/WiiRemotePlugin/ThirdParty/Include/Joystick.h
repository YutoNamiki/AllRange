#pragma once

#include "Utility.h"

struct Joystick
{
	Vector2D<float> RawData;
	Vector2D<float> Data;
	Vector2D<float> DeadZone;
};
