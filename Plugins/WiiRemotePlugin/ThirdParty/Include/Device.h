#pragma once

enum class RegisterDevice : int
{
	Calibration = 0x0016,
	IR = 0x4b00030,
	IRSensitivity1 = 0x4b00000,
	IRSensitivity2 = 0x4b0001a,
	IRMode = 0x4b00033,
	ExtensionInitialize1 = 0x4a400f0,
	ExtensionInitialize2 = 0x4a400fb,
	ExtensionType = 0x4a400fa,
	ExtensionCalibration = 0x4a40020,
	BalanceCalibration = 0x4a40024,
	MotionPlusDetect = 0x4a600fa,
	MotionPlusInitialize = 0x4a600f0,
	MotionPlusEnable = 0x4a600fe,
};

enum class AddressDevice : unsigned long long
{
	Nunchuk = 0x000020A40000ULL,
	Classic = 0x010120A40000ULL,
	GH3GhwtGuitar = 0x030120A40000ULL,
	GhwtDrums = 0x030120A40001ULL,
	BalanceBoard = 0x020420A40000ULL,
	MotionPlus = 0x050420A40000ULL,
	MotionPlusDetect = 0x050020a60000ULL,
	MotionPlusDetect2 = 0x050420a60000ULL,
	WiiRemotePlus = 0x050420A40001ULL,
	WiiRemotePlusDetect = 0x050020A60001ULL,
	WiiRemotePlusDetect2 = 0x050420A60001ULL,
	PartiallyInserted = 0xFFFFFFFFFFFFULL,
};
