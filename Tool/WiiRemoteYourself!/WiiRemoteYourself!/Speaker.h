#pragma once

enum class SpeakerFrequency : unsigned char
{
	FrequencyNone,
	Frequency4200Hz,
	Frequency3920Hz,
	Frequency3640Hz,
	Frequency3360Hz,
	Frequency3130Hz,
	Frequency2940Hz,
	Frequency2760Hz,
	Frequency2610Hz,
	Frequency2470Hz
};

struct Speaker
{
	bool Enabled;
	bool Muted;
	SpeakerFrequency Frequency;
	unsigned char Volume;
};

struct RiffChunkHeader
{
	char ChunkID[4];
	unsigned long ChunkSize;
	char FormType[4];
};

struct ChunkHeader
{
	char ChunkID[4];
	unsigned long ChunkSize;
};
