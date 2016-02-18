#pragma once

#include "Report.h"
#include <string>

struct QueueEntry
{
	unsigned char Report[ReportLength];

	QueueEntry() { memset(Report, 0, sizeof(Report)); }
};

struct HID
{
	static const unsigned int MaxQueueEntries = 1 << 7;
	QueueEntry* Queue;
	unsigned int ReadIndex;
	unsigned int WriteIndex;
	
	HID() : Queue(nullptr), ReadIndex(0), WriteIndex(0) {}

	bool IsEmpty() const { return (ReadIndex == WriteIndex); }
	bool Allocate();
	void Deallocate();
};
