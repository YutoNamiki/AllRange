#pragma once

static const int ReportLength = 22;

enum class InputReport : unsigned char
{
	Status = 0x20,
	ReadAddress = 0x21,
	Buttons = 0x30,
	ButtonsAcceleration = 0x31,
	ButtonsAccelerationIR = 0x33,
	ButtonsAccelerationExtension = 0x35,
	ButtonsAccelerationIRExtension = 0x37,
	ButtonsBalanceBoard = 0x32,
};

enum class OutputReport : unsigned char
{
	None = 0x00,
	LEDs = 0x11,
	Type = 0x12,
	IR = 0x13,
	SpeakerEnable = 0x14,
	Status = 0x15,
	WriteMemory = 0x16,
	ReadMemory = 0x17,
	SpeakerData = 0x18,
	SpeakerMute = 0x19,
	IR2 = 0x1A,
};
