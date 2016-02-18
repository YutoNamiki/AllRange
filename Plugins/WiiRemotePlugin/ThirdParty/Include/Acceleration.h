#pragma once

#include "Utility.h"

struct Orientation
{
	Vector<float> Data;
	unsigned int UpdateAge;
	Rotator<float> Rotation;
};

struct Acceleration
{
	Vector<unsigned char> RawData;
	Vector<float> Data;
	Orientation Orientation;
};
