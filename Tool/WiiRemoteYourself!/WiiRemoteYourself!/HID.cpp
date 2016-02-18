#include "WiiRemoteYourselfPCH.h"
#include "HID.h"

bool HID::Allocate()
{
	if (Queue)
		return true;
	ReadIndex = WriteIndex = 0;
	Queue = new QueueEntry[MaxQueueEntries];
	return (Queue != nullptr);
}

void HID::Deallocate()
{
	if (!Queue)
		return;
	delete[] Queue;
	Queue = nullptr;
	ReadIndex = WriteIndex = 0;
}
