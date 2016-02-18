#pragma once

#include "Speaker.h"

struct WiiRemoteSample
{
	unsigned char* Samples;
	unsigned long Length;
	SpeakerFrequency Frequency;

	WiiRemoteSample() : Samples(nullptr), Length(0), Frequency(SpeakerFrequency::FrequencyNone) {}

};
