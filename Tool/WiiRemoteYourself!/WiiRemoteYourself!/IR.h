#pragma once

#include "Utility.h"

enum class IRMode : unsigned char
{
	Off = 0x00,
	Basic = 0x01,
	Extended = 0x03,
	Full = 0x05,
};

struct Dot
{
	bool Visible;
	Vector2D<unsigned int> RawData;
	Vector2D<float> Data;
	int Size;
};

struct IR
{
	static const unsigned int MaxRawX = 1023;
	static const unsigned int MaxRawY = 767;

	IRMode Mode;
	Dot Dot[4];
};
