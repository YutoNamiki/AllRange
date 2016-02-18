#pragma once

#include "Utility.h"

struct SensorsRaw
{
	short TopR;
	short TopL;
	short BottomR;
	short BottomL;
};

struct SensorsFloating
{
	float TopR;
	float TopL;
	float BottomR;
	float BottomL;
	float Total;
};

struct WiiBalanceBoardCalibrationInformation
{
	SensorsRaw Kg0;
	SensorsRaw Kg17;
	SensorsRaw Kg34;
};

struct WiiBalanceBoard
{
	WiiBalanceBoardCalibrationInformation CalibrationInformation;
	SensorsRaw Raw;
	SensorsFloating AtRestKg;
	SensorsFloating Kg;
	SensorsFloating Lb;
};
