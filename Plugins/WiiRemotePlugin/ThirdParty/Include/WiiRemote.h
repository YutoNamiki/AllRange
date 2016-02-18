#pragma once

#define USE_DYNAMIC_HIDQUEUE

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "hid.lib")
#pragma comment(lib, "winmm.lib")

#include "StateChangeFlags.h"
#include "WiiRemoteState.h"
#include "Report.h"
#include <Windows.h>
#include <list>
#include <mutex>

#ifdef USE_DYNAMIC_HIDQUEUE
#include <queue>
#else
#include "HID.h"
#endif

namespace std
{
	class thread;
}
struct Acceleration;
struct WiiRemoteSample;

using StateChangedCallback = void(*)(class WiiRemote& owner, const StateChangeFlags changed, const WiiRemoteState& newState);
using HidWritePtr = unsigned char(*)(void* hidDeviceObject, void* reportBuffer, const unsigned long reportBufferLength);

struct Recording
{
	volatile bool Enabled;
	std::list<WiiRemoteStateEvent>* StateHistory;
	volatile unsigned long StartTimeMS;
	volatile unsigned long EndTimeMS;
	unsigned int TriggerFlags;
	unsigned int ExtTriggerFlags;
};

class WiiRemote : public WiiRemoteState
{
public:
	unsigned long long UniqueID;
	unsigned long long UniqueID2;
	StateChangedCallback ChangedCallback;
	StateChangeFlags CallbackTriggerFlags;

	static const unsigned int FirstAvailable = 0xffffffff;
	static const unsigned int UntilStop = 0xffffffff;

	WiiRemote();
	virtual ~WiiRemote();

	bool operator == (const WiiRemote& remote) { return handle == remote.handle; }
	bool operator != (const WiiRemote& remote) { return handle != remote.handle; }

	bool Connect(const unsigned int wiiRemoteIndex = FirstAvailable, bool forceHidwrites = false);
	void Disconnect();
	void SetReportType(const InputReport type, const bool continuous = false);
	bool EnableMotionPlus();
	bool DisableMotionPlus();
	void CalibrateAtRest();
	StateChangeFlags RefreshState();
	void Reset();
	void SetLEDs(const unsigned char ledBits);
	void SetRumble(const bool on);
	void RumbleForAsync(const unsigned int milliseconds);
	bool MuteSpeaker(const bool on);
	bool EnableSpeaker(const bool on);
	bool PlaySquareWave(const SpeakerFrequency frequency, const unsigned char volume = 0x40);
	bool PlaySample(const WiiRemoteSample& sample, const unsigned char volume = 0x40, const SpeakerFrequency frequencyOverride = SpeakerFrequency::FrequencyNone);
	void RecordState(std::list<WiiRemoteStateEvent>& eventsOut, unsigned int maxTimeMs = UntilStop, StateChangeFlags changeTrigger = StateChangeFlags::ChangedAll);
	void StopRecording();

	bool IsConnected() const { return bStatusReceived; }
	bool ConnectionLost() const { return bConnectionLost; }
	bool IsBalanceBoard() const { return (internal.bExtension && (internal.ExtensionType == ExtensionType::BalanceBoard)); }
	bool NunchukConnected() const { return (internal.bExtension && (internal.ExtensionType == ExtensionType::Nunchuk)); }
	bool ClassicConnected() const { return (internal.bExtension && (internal.ExtensionType == ExtensionType::Classic)); }
	bool MotionPlusConnected() const { return bMotionPlusDetected; }
	bool MotionPlusEnabled() const { return bMotionPlusEnabled; }
	bool MotionPlusHasExtension() const { return bMotionPlusExtension; }
	bool IsPlayingAudio() const { return ((internal.Speaker.Frequency != SpeakerFrequency::FrequencyNone) && internal.Speaker.Volume); }
	bool IsPlayingSample() const { return IsPlayingAudio() && (currentSample != nullptr); }
	bool IsUsingHIDWrites() const { return bUseHIDWrite; }
	bool IsRecordingState() const { return recording.Enabled; }
	
	static unsigned int TotalConnected() { return totalConnected; }

	virtual void ChangedNotifier(const StateChangeFlags changed, const WiiRemoteState& newState) { }

	static const char* GetButtonNameFromBit(const unsigned int index);
	static const char* GetClassicButtonNameFromBit(const unsigned int index);
	static const unsigned int GetFrequencyLookup(const unsigned int index);
	static bool Load16BitMonoSampleWAV(const char* filepath, WiiRemoteSample& out);
	static bool Load16BitMonoSampleRAW(const char* filepath, const bool signedFlag, const SpeakerFrequency frequency, WiiRemoteSample& out);
	static bool Convert16BitMonoSamples(const short* samples, const bool signedFlag, const unsigned long length, const SpeakerFrequency frequency, WiiRemoteSample& out);

private:

	static const int VID = 0x057E;	// 'Nintendo'
	static const int PID = 0x0306;	// 'Wiimote'
	static const int requestStatusEveryMs = 1000;
	static const int detectMotionPlusEveryMs = 1000;
	static const int detectMotionPlusCount = 1;
	
	void* handle;
	OVERLAPPED overlapped;
	std::thread* readParseThread;
	void* dataRead;
	bool bUseHIDWrite;

	static HMODULE hidDll;
	static HidWritePtr hidDSetOutputReport;

	volatile bool bStatusReceived;
	volatile bool bConnectInProgress;
	volatile bool bInitInProgress;
	volatile bool bEnablingMotionPlus;
	volatile bool bConnectionLost;
	volatile int motionPlusDetectCount;
	volatile bool bMotionPlusDetected;
	volatile bool bMotionPlusEnabled;
	volatile bool bMotionPlusExtension;
	volatile bool bCalibrateAtRest;
	static unsigned int totalCreated;
	static unsigned int totalConnected;
	InputReport reportType;
	unsigned char readBuffer[ReportLength];
	std::mutex stateLock;
	WiiRemoteState internal;
	StateChangeFlags internalChanged;
	unsigned long nextStatusTime;
	unsigned long nextMPlusDetectTime;
	unsigned long mPlusDetectCount;
	std::thread* hidWriteThread;
#ifdef USE_DYNAMIC_HIDQUEUE
	std::queue<unsigned char*> hidWriteQueue;
#else
	HID hid;
#endif
	std::mutex hidWriteQueueLock;
	std::thread* asyncRumbleThread;
	volatile unsigned long asyncRumbleTimeout;
	unsigned long wiiRemoteNearGravityUpdates;
	unsigned long wiiNunchukNEarGravityUpdates;
	std::thread* sampleThread;
	const WiiRemoteSample* volatile currentSample;
	Recording recording;

	static const int TotalFrequencies = 10;
	static const int TotalButtonBits = 16;
	static const char* ButtonNameFromBit[TotalButtonBits];
	static const char* ClassicButtonNameFromBit[TotalButtonBits];
	static const unsigned int FrequencyLookup[TotalFrequencies];

	bool BeginAsyncRead();
	void RequestStatusReport();
	bool ReadAddress(const int address, const short size);
	void WriteData(const int address, const unsigned char data) { WriteData(address, 1, &data); }
	void WriteData(const int address, const unsigned char size, const unsigned char* buffer);
	void OnReadData(const unsigned long bytesRead);
	bool ParseInput(unsigned char* buffer);
	void DetectMotionPlusExtensionAsync();
	void InitializeExtension();
	int ParseStatus(unsigned char* buffer);
	int ParseButtons(unsigned char* buffer);
	int ParseAcceleration(unsigned char* buffer);
	bool EstimateOrientationFrom(struct Acceleration& acceleration);
	void ApplyJoystickDeadZones(Joystick& joystick);
	int ParseIR(unsigned char* buffer);
	int ParseExtension(unsigned char* buffer, const unsigned int offset);
	int ParseReadAddress(unsigned char* buffer);
	void ReadCalibration();
	float GetBalanceValue(const short sensor, const short minimum, const short middle, const short maximum);
	void EnableIR(const IRMode mode);
	void DisableIR();
	bool WriteReport(const unsigned char* buffer);
	bool StartSampleThread();

	unsigned char GetRumbleBit() const { return internal.bRumble ? 0x01 : 0x00; }

	int ParseNunchuk(unsigned char* buffer, const unsigned int offset);
	int ParseClassicController(unsigned char* buffer, const unsigned int offset);
	int ParseBalanceBoard(unsigned char* buffer, const unsigned int offset);
	int ParseMotionPlus(unsigned char* buffer, const unsigned int offset);

	int ReadDeviceAddress(unsigned char* buffer);
	int CalibrationDevice(unsigned char* buffer);

	static unsigned int ReadParseThreadFunction(WiiRemote* param);
	static unsigned int AsyncRumleThreadFunction(WiiRemote* param);
	static unsigned int SampleStreamThreadFunction(WiiRemote* param);
	static unsigned int HIDWriteThreadFunction(WiiRemote* param);

};
