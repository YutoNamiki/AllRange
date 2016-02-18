#pragma once

#include "Utility.h"

struct WiiMotionPlus
{
	Rotator<short> Raw;
	Rotator<float> Speed;
};
