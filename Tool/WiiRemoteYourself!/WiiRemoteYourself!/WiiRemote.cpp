#include "WiiRemoteYourselfPCH.h"
#include "WiiRemote.h"
#include "WiiRemoteSample.h"
#include "Device.h"
#include <hidsdi.h>
#include <mmreg.h>
#include <mmsystem.h>
#include <SetupAPI.h>
#include <tchar.h>
#include <algorithm>
#include <thread>

HMODULE WiiRemote::hidDll = nullptr;
unsigned int WiiRemote::totalCreated = 0;
unsigned int WiiRemote::totalConnected = 0;
HidWritePtr WiiRemote::hidDSetOutputReport = nullptr;

const char* WiiRemote::ButtonNameFromBit[] =
{
	"Left", "Right", "Down", "Up", "Plus", "??", "??", "??",
	"Two", "One", "B", "A", "Minus", "??", "??", "Home"
};
const char* WiiRemote::ClassicButtonNameFromBit[] = 
{
	"??", "TrigR", "Plus", "Home", "Minus", "TrigL", "Down", "Right",
	"Up", "Left", "ZR", "X", "A", "Y", "B", "ZL"
};
const unsigned int WiiRemote::FrequencyLookup[] =
{
	0, 4200, 3920, 3640, 3360, 3130, 2940, 2760, 2610, 2470
};

WiiRemote::WiiRemote() :
	dataRead(CreateEvent(nullptr, FALSE, FALSE, nullptr)),
	handle(INVALID_HANDLE_VALUE),
	reportType(InputReport::Buttons),
	bStatusReceived(false),
	bConnectInProgress(false),
	bInitInProgress(false),
	bEnablingMotionPlus(false),
	bConnectionLost(false),
	bMotionPlusDetected(false),
	bMotionPlusEnabled(false),
	bMotionPlusExtension(false),
	bCalibrateAtRest(false),
	bUseHIDWrite(false),
	ChangedCallback(nullptr),
	CallbackTriggerFlags(StateChangeFlags::ChangedAll),
	internalChanged(StateChangeFlags::NoChange),
	currentSample(nullptr),
	hidWriteThread(nullptr),
	readParseThread(nullptr),
	sampleThread(nullptr),
	asyncRumbleThread(nullptr),
	asyncRumbleTimeout(0),
	UniqueID(0),
	UniqueID2(0)
{
	if (++totalCreated == 1)
	{
		hidDll = LoadLibraryA("hid.dll");
		if (hidDll)
			hidDSetOutputReport = reinterpret_cast<HidWritePtr>(GetProcAddress(hidDll, "HidD_SetOutputReport"));
	}
	Clear(true);
	internal.Clear(true);
	memset(&recording, 0, sizeof(recording));
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = dataRead;
	overlapped.Offset = overlapped.OffsetHigh = 0;
	timeBeginPeriod(1);
}

WiiRemote::~WiiRemote()
{
	Disconnect();
	if (dataRead != INVALID_HANDLE_VALUE)
		CloseHandle(dataRead);
	timeEndPeriod(1);
	if ((--totalCreated == 0) && hidDll)
	{
		FreeLibrary(hidDll);
		hidDll = nullptr;
		hidDSetOutputReport = nullptr;
	}
}

bool WiiRemote::Connect(const unsigned int wiiRemoteIndex, bool forceHidwrites)
{
	if (IsConnected())
		Disconnect();
	GUID guid;
	HidD_GetHidGuid(&guid);
	auto devInfo = SetupDiGetClassDevs(&guid, nullptr, nullptr, DIGCF_DEVICEINTERFACE);
	if (!devInfo)
		return false;
	SP_DEVICE_INTERFACE_DATA diData;
	diData.cbSize = sizeof(diData);
	auto index = 0;
	auto wiiRemotesFounds = 0;
	while (SetupDiEnumDeviceInterfaces(devInfo, nullptr, &guid, index, &diData))
	{
		DWORD reqSize = 0;
		SetupDiGetDeviceInterfaceDetail(devInfo, &diData, nullptr, 0, &reqSize, nullptr);
		auto diDetail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA*>(new unsigned char[reqSize]);
		diDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (!SetupDiGetDeviceInterfaceDetail(devInfo, &diData, diDetail, reqSize, &reqSize, nullptr))
			break;
		handle = CreateFile(diDetail->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
		if (handle == INVALID_HANDLE_VALUE)
		{
			delete[] reinterpret_cast<unsigned char*>(diDetail);
			if (handle != INVALID_HANDLE_VALUE)
			{
				CloseHandle(handle);
				handle = INVALID_HANDLE_VALUE;
			}
			if ((wiiRemoteIndex != FirstAvailable) && (wiiRemoteIndex == (wiiRemotesFounds - 1)))
				break;
			index++;
			continue;
		}
		HIDD_ATTRIBUTES attribute;
		attribute.Size = sizeof(attribute);
		if (HidD_GetAttributes(handle, &attribute))
		{
			if ((attribute.VendorID != VID) || (attribute.ProductID != PID))
			{
				delete[] reinterpret_cast<unsigned char*>(diDetail);
				if (handle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(handle);
					handle = INVALID_HANDLE_VALUE;
				}
				if ((wiiRemoteIndex != FirstAvailable) && (wiiRemoteIndex == (wiiRemotesFounds - 1)))
					break;
				index++;
				continue;
			}
			++wiiRemotesFounds;
			if ((wiiRemoteIndex != FirstAvailable) && (wiiRemoteIndex != wiiRemotesFounds))
			{
				delete[] reinterpret_cast<unsigned char*>(diDetail);
				if (handle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(handle);
					handle = INVALID_HANDLE_VALUE;
				}
				if ((wiiRemoteIndex != FirstAvailable) && (wiiRemoteIndex == (wiiRemotesFounds - 1)))
					break;
				index++;
				continue;
			}
			CloseHandle(handle);
			handle = CreateFile(diDetail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
			if (handle == INVALID_HANDLE_VALUE)
			{
				delete[] reinterpret_cast<unsigned char*>(diDetail);
				if (handle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(handle);
					handle = INVALID_HANDLE_VALUE;
				}
				if ((wiiRemoteIndex != FirstAvailable) && (wiiRemoteIndex == (wiiRemotesFounds - 1)))
					break;
				index++;
				continue;
			}
			Clear(false);
			internal.Clear(false);
			internalChanged = StateChangeFlags::NoChange;
			memset(readBuffer, 0, sizeof(readBuffer));
			bConnectionLost = false;
			bConnectInProgress = true;
			BeginAsyncRead();
			if (forceHidwrites && !hidDSetOutputReport)
				forceHidwrites = false;
			if (!forceHidwrites)
			{
				bUseHIDWrite = false;
				RequestStatusReport();
				auto lastTime = timeGetTime();
				while (!bStatusReceived && ((timeGetTime() - lastTime) < 500))
					Sleep(10);
			}
			if (!bStatusReceived && hidDSetOutputReport)
			{
				bUseHIDWrite = true;
				RequestStatusReport();
				auto lastTime = timeGetTime();
				while (!bStatusReceived && ((timeGetTime() - lastTime) < 500))
					Sleep(10);
			}
			if (!bStatusReceived)
			{
				Disconnect();
				delete[] reinterpret_cast<unsigned char*>(diDetail);
				if (handle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(handle);
					handle = INVALID_HANDLE_VALUE;
				}
				if ((wiiRemoteIndex != FirstAvailable) && (wiiRemoteIndex == (wiiRemotesFounds - 1)))
					break;
				index++;
				continue;
			}
			Reset();
			ReadCalibration();
			Sleep(300);
			totalConnected++;
			bCalibrateAtRest = true;
			RefreshState();
			UniqueID = reinterpret_cast<unsigned long long>(&CalibrationInformation);
			UniqueID2 = 0;
			for (auto index = 0; index < static_cast<int>(_tcslen(diDetail->DevicePath)); index++)
				UniqueID2 += diDetail->DevicePath[index];
			nextStatusTime = timeGetTime() + requestStatusEveryMs;
			nextMPlusDetectTime = timeGetTime() + detectMotionPlusEveryMs;
			mPlusDetectCount = detectMotionPlusCount;
			delete[] reinterpret_cast<unsigned char*>(diDetail);
			break;
		}
		delete[] reinterpret_cast<unsigned char*>(diDetail);
		if (handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
		}
		if ((wiiRemoteIndex != FirstAvailable) && (wiiRemoteIndex == (wiiRemotesFounds - 1)))
			break;
		index++;
	}
	SetupDiDestroyDeviceInfoList(devInfo);
	bConnectInProgress = false;
	if (IsConnected())
	{
		if (static_cast<int>(CallbackTriggerFlags) & static_cast<int>(StateChangeFlags::Connected))
		{
			ChangedNotifier(StateChangeFlags::Connected, internal);
			if (ChangedCallback)
				ChangedCallback(*this, StateChangeFlags::Connected, internal);
		}
		return true;
	}
	return false;
}

void WiiRemote::Disconnect()
{
	if (handle == INVALID_HANDLE_VALUE)
		return;
	if (IsConnected())
	{
		totalConnected--;
		if (!bConnectionLost)
			Reset();
	}
	CloseHandle(handle);
	handle = INVALID_HANDLE_VALUE;
	UniqueID = 0;
	UniqueID2 = 0;
	bStatusReceived = false;
	if (readParseThread)
	{
		SetEvent(dataRead);
		readParseThread->join();
		delete readParseThread;
		readParseThread = nullptr;
	}
	if (asyncRumbleThread)
	{
		asyncRumbleThread->join();
		delete asyncRumbleThread;
		asyncRumbleThread = nullptr;
		asyncRumbleTimeout = 0;
	}
	if (sampleThread)
	{
		sampleThread->join();
		delete sampleThread;
		sampleThread = nullptr;
	}
#ifndef USE_DYNAMIC_HIDQUEUE
	hid.Deallocate();
#endif
	Clear(false);
	internal.Clear(false);
	internalChanged = StateChangeFlags::NoChange;
}

void WiiRemote::SetReportType(const InputReport type, const bool continuous)
{
	if (!IsConnected())
		return;
	if (IsBalanceBoard() && (type != InputReport::ButtonsBalanceBoard))
		return;
	reportType = type;
	switch (type)
	{
	case InputReport::ButtonsAccelerationIR:
		EnableIR(IRMode::Extended);
		break;
	case InputReport::ButtonsAccelerationIRExtension:
		EnableIR(IRMode::Basic);
		break;
	default:
		DisableIR();
		break;
	}
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::Type);
	buffer[1] = (continuous ? 0x04 : 0x00) | GetRumbleBit();
	buffer[2] = static_cast<unsigned char>(type);
	WriteReport(buffer);
}

bool WiiRemote::EnableMotionPlus()
{
	if (!bMotionPlusDetected)
		return false;
	if (bMotionPlusEnabled)
		return true;
	bMotionPlusExtension = false;
	bInitInProgress = true;
	bEnablingMotionPlus = true;
	WriteData(static_cast<int>(RegisterDevice::MotionPlusInitialize), 0x55);
	WriteData(static_cast<int>(RegisterDevice::MotionPlusEnable), 0x04);
	Sleep(500);
	return true;
}

bool WiiRemote::DisableMotionPlus()
{
	if (!bMotionPlusDetected || !bMotionPlusEnabled)
		return false;
	WriteData(static_cast<int>(RegisterDevice::ExtensionInitialize1), 0x55);
	return true;
}

void WiiRemote::CalibrateAtRest()
{
	if (!IsConnected())
		return;
	if (IsBalanceBoard())
	{
		internal.BalanceBoard.AtRestKg = internal.BalanceBoard.Kg;
		RefreshState();
	}
}

StateChangeFlags WiiRemote::RefreshState()
{
	if (internalChanged == StateChangeFlags::NoChange)
		return StateChangeFlags::NoChange;
	auto lock = new std::lock_guard<std::mutex>(stateLock);
	const auto changed = internalChanged;
	const auto nunchukDeadzone = Nunchuk.Joystick.DeadZone;
	const auto classicJoyLDeadzone = ClassicController.JoystickL.DeadZone;
	const auto classicJoyRDeadzone = ClassicController.JoystickR.DeadZone;
	*static_cast<WiiRemoteState*>(this) = internal;
	internalChanged = StateChangeFlags::NoChange;
	Nunchuk.Joystick.DeadZone = nunchukDeadzone;
	ClassicController.JoystickL.DeadZone = classicJoyLDeadzone;
	ClassicController.JoystickR.DeadZone = classicJoyRDeadzone;
	delete lock;
	return changed;
}

void WiiRemote::Reset()
{
	if (bMotionPlusEnabled)
		DisableMotionPlus();
	if (IsBalanceBoard())
		SetReportType(InputReport::ButtonsBalanceBoard, false);
	else
		SetReportType(InputReport::Buttons, false);
	SetRumble(false);
	SetLEDs(0x00);
	EnableSpeaker(false);
	Sleep(150);
}

void WiiRemote::SetLEDs(const unsigned char ledBits)
{
	if (!IsConnected())
		return;
	const auto bits = ledBits & 0xf;
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::LEDs);
	buffer[1] = (bits << 4) | GetRumbleBit();
	WriteReport(buffer);
	internal.LED.Bits = bits;
}

void WiiRemote::SetRumble(const bool on)
{
	if (!IsConnected())
		return;
	if (internal.bRumble == on)
		return;
	internal.bRumble = on;
	if (IsPlayingAudio())
		return;
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::Status);
	buffer[1] = on ? 0x01 : 0x00;
	WriteReport(buffer);
}

void WiiRemote::RumbleForAsync(const unsigned int milliseconds)
{
	if (!IsConnected())
		return;
	SetRumble(true);
	asyncRumbleTimeout = timeGetTime() + milliseconds;
	if (asyncRumbleThread)
		return;
	asyncRumbleThread = new std::thread(AsyncRumleThreadFunction, this);
	if (!asyncRumbleThread)
		return;
}

bool WiiRemote::MuteSpeaker(const bool on)
{
	if (!IsConnected())
		return false;
	if (internal.Speaker.Enabled == on)
		return true;
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::SpeakerMute);
	buffer[1] = (on ? 0x04 : 0x00) | GetRumbleBit();
	if (!WriteReport(buffer))
		return false;
	Sleep(1);
	internal.Speaker.Muted = on;
	return true;
}

bool WiiRemote::EnableSpeaker(const bool on)
{
	if (!IsConnected())
		return false;
	if (internal.Speaker.Enabled == on)
		return true;
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::SpeakerEnable);
	buffer[1] = (on ? 0x04 : 0x00) | GetRumbleBit();
	if (!WriteReport(buffer))
		return false;
	if (!on)
	{
		internal.Speaker.Frequency = SpeakerFrequency::FrequencyNone;
		internal.Speaker.Volume = 0;
		MuteSpeaker(true);
	}
	internal.Speaker.Enabled = on;
	return true;
}

bool WiiRemote::PlaySquareWave(const SpeakerFrequency frequency, const unsigned char volume)
{
	if (!IsConnected())
		return false;
	if (IsPlayingSample())
		currentSample = nullptr;
	else if (IsPlayingAudio() && (internal.Speaker.Frequency == frequency) && (internal.Speaker.Volume == volume))
		return true;
	currentSample = 0;
	EnableSpeaker(true);
	MuteSpeaker(true);
	WriteData(0x04A20009, 0x01);
	WriteData(0x04A20001, 0x08);
	unsigned char bytes[7] = 
	{ 
		static_cast<unsigned char>(0x00),
		static_cast<unsigned char>(0x00), 
		static_cast<unsigned char>(0x00), 
		static_cast<unsigned char>(10 + static_cast<int>(frequency)), 
		volume, 
		static_cast<unsigned char>(0x00),
		static_cast<unsigned char>(0x00) 
	};
	WriteData(0x04A20001, static_cast<unsigned char>(sizeof(bytes)), bytes);
	WriteData(0x04A20008, 0x01);
	internal.Speaker.Frequency = frequency;
	internal.Speaker.Volume = volume;
	MuteSpeaker(false);
	return StartSampleThread();
}

bool WiiRemote::PlaySample(const WiiRemoteSample& sample, const unsigned char volume, const SpeakerFrequency frequencyOverride)
{
	if (!IsConnected())
		return false;
	const auto frequency = (frequencyOverride != SpeakerFrequency::FrequencyNone) ? frequencyOverride : sample.Frequency;
	EnableSpeaker(true);
	MuteSpeaker(true);
	WriteData(0x04A20009, 0x01);
	WriteData(0x04A20001, 0x08);
	unsigned char bytes[7] = 
	{
		static_cast<unsigned char>(0x00),
		static_cast<unsigned char>(0x00),
		static_cast<unsigned char>(0x00),
		static_cast<unsigned char>(10 + static_cast<int>(frequency)),
		volume,
		static_cast<unsigned char>(0x00),
		static_cast<unsigned char>(0x00)
	};
	WriteData(0x04A20001, static_cast<unsigned char>(sizeof(bytes)), bytes);
	WriteData(0x04A20008, 0x01);
	internal.Speaker.Frequency = frequency;
	internal.Speaker.Volume = volume;
	currentSample = &sample;
	MuteSpeaker(false);
	return StartSampleThread();
}

void WiiRemote::RecordState(std::list<WiiRemoteStateEvent>& eventsOut, unsigned int maxTimeMs, StateChangeFlags changeTrigger)
{
	if (recording.Enabled)
		StopRecording();
	if (!eventsOut.empty())
		eventsOut.clear();
	recording.StateHistory = &eventsOut;
	recording.StartTimeMS = timeGetTime();
	recording.EndTimeMS = recording.StartTimeMS + maxTimeMs;
	recording.TriggerFlags = static_cast<unsigned int>(changeTrigger);
	recording.Enabled = true;
}

void WiiRemote::StopRecording()
{
	if (!recording.Enabled)
		return;
	recording.Enabled = false;
	Sleep(10);
}

const char* WiiRemote::GetButtonNameFromBit(const unsigned int index)
{
	if (index >= TotalButtonBits)
		return "[invalid index]";
	return ButtonNameFromBit[index];
}

const char* WiiRemote::GetClassicButtonNameFromBit(const unsigned int index)
{
	if (index >= TotalButtonBits)
		return "[invalid index]";
	return ClassicButtonNameFromBit[index];
}

const unsigned int WiiRemote::GetFrequencyLookup(const unsigned int index)
{
	if (index >= TotalFrequencies)
		return 0;
	return FrequencyLookup[index];
}

bool WiiRemote::Load16BitMonoSampleWAV(const char* filepath, WiiRemoteSample& out)
{
	memset(&out, 0, sizeof(out));
	FILE* file = nullptr;
	fopen_s(&file, filepath, "rb");
	if (!file)
		return false;
	union {
		WAVEFORMATEX x;
		WAVEFORMATEXTENSIBLE xe;
	} waveFormat = { 0 };
	RiffChunkHeader riffChunkHeader;
	ChunkHeader chunkHeader;
	auto frequency = SpeakerFrequency::FrequencyNone;
	if (fread_s(&riffChunkHeader, sizeof(riffChunkHeader), sizeof(riffChunkHeader), 1, file) != 1)
	{
		fclose(file);
		return false;
	}
	if (strncmp(riffChunkHeader.ChunkID, "RIFF", 4))
	{
		fclose(file);
		return false;
	}
	if (strncmp(riffChunkHeader.FormType, "WAVE", 4))
	{
		fclose(file);
		return false;
	}
	while (1)
	{
		if (fread_s(&chunkHeader, sizeof(chunkHeader), sizeof(chunkHeader), 1, file) != 1)
		{
			fclose(file);
			return false;
		}
		if (!strncmp(chunkHeader.ChunkID, "fmt ", 4))
		{
			if (chunkHeader.ChunkSize < 16 || chunkHeader.ChunkSize > sizeof(WAVEFORMATEXTENSIBLE))
			{
				fclose(file);
				return false;
			}
			if (fread_s(reinterpret_cast<unsigned char*>(&waveFormat.x), chunkHeader.ChunkSize, chunkHeader.ChunkSize, 1, file) != 1)
			{
				fclose(file);
				return false;
			}
			const auto extensible = (waveFormat.x.wFormatTag == WAVE_FORMAT_EXTENSIBLE);
			const int format = extensible ? waveFormat.xe.SubFormat.Data1 : waveFormat.x.wFormatTag;
			if (format != WAVE_FORMAT_PCM)
			{
				fclose(file);
				return false;
			}
			if ((waveFormat.x.nChannels != 1) || (waveFormat.x.wBitsPerSample != 16))
			{
				fclose(file);
				return false;
			}
			const unsigned int sampleFrequency = waveFormat.x.nSamplesPerSec;
			const auto epsilon = 100;
			const auto freqLookupLength = sizeof(FrequencyLookup) / sizeof(FrequencyLookup[0]);
			for (auto index = 1; index < freqLookupLength; index++)
			{
				if ((sampleFrequency + epsilon) >= FrequencyLookup[index] && (sampleFrequency - epsilon) <= FrequencyLookup[index])
				{
					frequency = static_cast<SpeakerFrequency>(index);
					break;
				}
			}
			if (frequency == SpeakerFrequency::FrequencyNone)
			{
				fclose(file);
				return false;
			}
		}
		else if (!strncmp(chunkHeader.ChunkID, "data", 4))
		{
			if (!waveFormat.x.nBlockAlign)
			{
				fclose(file);
				return false;
			}
			unsigned int totalSamples = chunkHeader.ChunkSize / waveFormat.x.nBlockAlign;
			if (totalSamples == 0)
			{
				fclose(file);
				return false;
			}
			auto samples = new short[totalSamples];
			const auto read = fread_s(samples, sizeof(short) * totalSamples, 2, totalSamples, file);
			fclose(file);
			if (read != totalSamples)
			{
				if (read == 0)
				{
					delete[] samples;
					fclose(file);
					return false;
				}
				totalSamples = static_cast<unsigned int>(read);
			}
			const auto res = Convert16BitMonoSamples(samples, true, totalSamples, frequency, out);
			delete[] samples;
			return res;
		}
		else
		{
			const auto chunckBytes = (chunkHeader.ChunkSize + 1) & ~1L;
			if (fseek(file, chunckBytes, SEEK_CUR))
			{
				fclose(file);
				return false;
			}
		}
	}
	return false;
}

bool WiiRemote::Load16BitMonoSampleRAW(const char* filepath, const bool signedFlag, const SpeakerFrequency frequency, WiiRemoteSample& out)
{
	memset(&out, 0, sizeof(out));
	struct _stat64i32 fileInfo;
	if (_stat64i32(filepath, &fileInfo))
		return false;
	const auto len = fileInfo.st_size;
	if (!len)
		return false;
	const auto totalSamples = static_cast<unsigned int>((len + 1) * 0.5f);
	auto samples = new short[totalSamples];
	if (!samples)
		return false;
	FILE* file = nullptr;
	fopen_s(&file, filepath, "rb");
	if (!file)
	{
		delete[] samples;
		return false;
	}
	auto res = (fread_s(samples, sizeof(short) * totalSamples, 1, len, file) == len);
	fclose(file);
	if (!res)
	{
		delete[] samples;
		return false;
	}
	res = Convert16BitMonoSamples(samples, signedFlag, totalSamples, frequency, out);
	delete[] samples;
	return res;
}

bool WiiRemote::Convert16BitMonoSamples(const short* samples, const bool signedFlag, const unsigned long length, const SpeakerFrequency frequency, WiiRemoteSample& out)
{
	memset(&out, 0, sizeof(0));
	if (!samples || !length)
		return false;
	out.Samples = new unsigned char[length];
	if (!out.Samples)
		return false;
	memset(out.Samples, 0, length);
	out.Length = length;
	out.Frequency = frequency;
	static const int indexTable[16] = { -1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8 };
	static const int diffTable[16] = { 1, 3, 5, 7, 9, 11, 13, 15, -1, -3, -5, -7, -9, -11, -13, 15 };
	static const int stepScale[16] = { 230, 230, 230, 230, 307, 409, 512, 614, 230, 230, 230, 230, 307, 409, 512, 614 };
	auto adpcmPrevValue = 0;
	auto adpcmStep = 127;
	for (auto i = 0; i < static_cast<int>(length); i++)
	{
		auto value = samples[i];
		if (!signedFlag)
			value -= 32768;
		auto diff = value - adpcmPrevValue;
		unsigned char encodedValue = 0;
		if (diff < 0)
		{
			encodedValue |= 8;
			diff = -diff;
		}
		diff = (diff << 2) / adpcmStep;
		if (diff > 7)
			diff = 7;
		encodedValue |= diff;
		adpcmPrevValue += (adpcmStep * diffTable[encodedValue]) / 8;
		if (adpcmPrevValue > 0x7fff)
			adpcmPrevValue = 0x7fff;
		if (adpcmPrevValue < -0x8000)
			adpcmPrevValue = -0x8000;
		adpcmStep = (adpcmStep * stepScale[encodedValue]) >> 8;
		if (adpcmStep < 127)
			adpcmStep = 127;
		if (adpcmStep > 24567)
			adpcmStep = 24567;
		if (i & 1)
			out.Samples[i >> 1] |= encodedValue;
		else
			out.Samples[i >> 1] |= encodedValue << 4;
	}
	return true;
}

unsigned int WiiRemote::ReadParseThreadFunction(WiiRemote* param)
{
	auto& remote = *param;
	auto& overlapped = remote.overlapped;
	unsigned int exitCode = 0;
	while (1)
	{
		auto wait = WaitForSingleObject(overlapped.hEvent, 500);
		if (remote.handle == INVALID_HANDLE_VALUE)
			break;
		if (remote.bConnectionLost)
		{
			remote.Disconnect();
			remote.internalChanged = static_cast<StateChangeFlags>(static_cast<int>(remote.internalChanged) | static_cast<int>(StateChangeFlags::ConnectionLost));
			if (static_cast<int>(remote.CallbackTriggerFlags) & static_cast<int>(StateChangeFlags::ConnectionLost))
			{
				remote.ChangedNotifier(StateChangeFlags::ConnectionLost, remote.internal);
				if (remote.ChangedCallback)
					remote.ChangedCallback(remote, StateChangeFlags::ConnectionLost, remote.internal);
			}
			break;
		}
		const auto time = timeGetTime();
		if (remote.IsConnected() && !remote.bInitInProgress && !remote.IsPlayingAudio())
		{
			if (time > remote.nextStatusTime)
			{
				remote.RequestStatusReport();
				remote.nextStatusTime = time + requestStatusEveryMs;
			}
			if (!remote.IsBalanceBoard() && !remote.bMotionPlusExtension &&
				(remote.internal.ExtensionType != ExtensionType::MotionPlus) &&
				(remote.internal.ExtensionType != ExtensionType::WiiRemotePlus) &&
				(remote.internal.ExtensionType != ExtensionType::PartiallyInserted) &&
				(time > remote.nextMPlusDetectTime))
			{
				remote.DetectMotionPlusExtensionAsync();
				if (--remote.mPlusDetectCount == 0)
				{
					remote.nextMPlusDetectTime = time + detectMotionPlusEveryMs;
					remote.mPlusDetectCount = detectMotionPlusCount;
				}
			}
		}
		if (remote.recording.Enabled && (remote.recording.EndTimeMS != UntilStop) && (time >= remote.recording.EndTimeMS))
			remote.recording.Enabled = false;
		if (wait == WAIT_TIMEOUT)
			continue;
		if (wait != WAIT_OBJECT_0)
		{
			remote.bConnectionLost = true;
			remote.Disconnect();
			remote.internalChanged = static_cast<StateChangeFlags>(static_cast<int>(remote.internalChanged) | static_cast<int>(StateChangeFlags::ConnectionLost));
			if (static_cast<int>(remote.CallbackTriggerFlags) & static_cast<int>(StateChangeFlags::ConnectionLost))
			{
				remote.ChangedNotifier(StateChangeFlags::ConnectionLost, remote.internal);
				if (remote.ChangedCallback)
					remote.ChangedCallback(remote, StateChangeFlags::ConnectionLost, remote.internal);
			}
			break;
		}
		unsigned long read = 0;
		GetOverlappedResult(remote.handle, &overlapped, &read, TRUE);
		if (read)
			remote.OnReadData(read);
	}
	return exitCode;
}

unsigned int WiiRemote::AsyncRumleThreadFunction(WiiRemote* param)
{
	auto& remote = *param;
	while (remote.IsConnected())
	{
		if (remote.asyncRumbleTimeout)
		{
			const auto currentTime = timeGetTime();
			if (currentTime >= remote.asyncRumbleTimeout)
			{
				if (remote.internal.bRumble)
					remote.SetRumble(false);
				remote.asyncRumbleTimeout = 0;
			}
			Sleep(1);
		}
		else
			Sleep(4);
	}
	return 0;
}

unsigned int WiiRemote::SampleStreamThreadFunction(WiiRemote* param)
{
	auto& remote = *param;
	static unsigned char squarewaveReport[ReportLength] =
	{
		static_cast<unsigned char>(OutputReport::SpeakerData), 20 << 3,
		0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
		0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
	};
	static unsigned char sampleReport[ReportLength] = { static_cast<unsigned char>(OutputReport::SpeakerData), 0 };
	auto lastPlaying = false;
	unsigned long frame = 0, frameStart = 0;
	unsigned int totalSamples = 0, sampleIndex = 0;
	WiiRemoteSample* currentSample = nullptr;
	while (remote.IsConnected())
	{
		auto playing = remote.IsPlayingAudio();
		if (!playing)
			Sleep(1);
		else
		{
			const auto freqHz = FrequencyLookup[static_cast<int>(remote.internal.Speaker.Frequency)];
			const auto frameMs = 1000.0f / (freqHz / 40.0f);
			const auto sampleChanged = (currentSample != remote.currentSample);
			currentSample = const_cast<WiiRemoteSample*>(remote.currentSample);
			if (!lastPlaying || sampleChanged)
			{
				frame = 0;
				frameStart = timeGetTime();
				totalSamples = currentSample ? currentSample->Length : 0;
				sampleIndex = 0;
			}
			if (currentSample)
			{
				if (sampleIndex < currentSample->Length)
				{
					const auto samplesLeft = (currentSample->Length - sampleIndex);
					const auto reportSamples = std::min<unsigned int>(samplesLeft, 40);
					const auto reportEntries = (reportSamples + 1) >> 1;
					sampleReport[1] = static_cast<unsigned int>((reportEntries << 3) | remote.GetRumbleBit());
					for (auto index = 0; index < static_cast<int>(reportEntries); index++)
						sampleReport[2 + index] = currentSample->Samples[(sampleIndex >> 1) + index];
					remote.WriteReport(sampleReport);
					sampleIndex += reportSamples;
				}
				else
				{
					remote.currentSample = nullptr;
					currentSample = nullptr;
					remote.internal.Speaker.Frequency = SpeakerFrequency::FrequencyNone;
					remote.internal.Speaker.Volume = 0;
				}
			}
			else
			{
				squarewaveReport[1] = (20 << 3) | remote.GetRumbleBit();
				remote.WriteReport(squarewaveReport);
			}
			frame++;
			while ((timeGetTime() - frameStart) < static_cast<unsigned int>(frame * frameMs))
				Sleep(1);
		}
		lastPlaying = playing;
	}
	return 0;
}

unsigned int __stdcall WiiRemote::HIDWriteThreadFunction(WiiRemote* param)
{
	auto& remote = *param;
	while (remote.handle != INVALID_HANDLE_VALUE)
	{
#ifdef USE_DYNAMIC_HIDQUEUE
		if (!remote.hidWriteQueue.empty())
#else
		if (!remote.hid.IsEmpty())
#endif
		{
			auto lock = new std::lock_guard<std::mutex>(remote.hidWriteQueueLock);
#ifdef USE_DYNAMIC_HIDQUEUE
			auto buffer = remote.hidWriteQueue.front();
#else
			auto buffer = remote.hid.Queue[remote.hid.ReadIndex].Report;
#endif
			delete lock;
			if (!HidD_SetOutputReport(remote.handle, buffer, ReportLength))
			{
				auto error = GetLastError();
				if ((error != ERROR_BUSY) && (error != ERROR_NOT_READY))
				{
					if (error == ERROR_NOT_SUPPORTED)
					{
						std::lock_guard<std::mutex> lock(remote.hidWriteQueueLock);
#ifdef USE_DYNAMIC_HIDQUEUE
						remote.hidWriteQueue.pop();
						delete[] buffer;
#else
						remote.hid.ReadIndex++;
						remote.hid.ReadIndex &= (HID::MaxQueueEntries - 1);
#endif
					}
					else
					{
						if (remote.IsConnected())
							remote.bConnectionLost = true;
					}
				}
			}
			else
			{
				std::lock_guard<std::mutex> lock(remote.hidWriteQueueLock);
#ifdef USE_DYNAMIC_HIDQUEUE
				remote.hidWriteQueue.pop();
				delete[] buffer;
#else
				remote.hid.ReadIndex++;
				remote.hid.ReadIndex &= (HID::MaxQueueEntries - 1);
#endif
			}
		}
		Sleep(1);
	}
	return 0;
}

bool WiiRemote::BeginAsyncRead()
{
	if (handle == INVALID_HANDLE_VALUE)
		return false;
	DWORD read;
	if (!ReadFile(handle, readBuffer, ReportLength, &read, &overlapped))
	{
		auto error = GetLastError();
		if (error != ERROR_IO_PENDING)
			return false;
	}
	if (!readParseThread)
	{
		readParseThread = new std::thread(ReadParseThreadFunction, this);
		if (!readParseThread)
			return false;
	}
	if (read)
		SetEvent(dataRead);
	return true;
}

void WiiRemote::RequestStatusReport()
{
	if (handle == INVALID_HANDLE_VALUE)
		return;
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::Status);
	buffer[1] = static_cast<unsigned char>(GetRumbleBit());
	WriteReport(buffer);
}

bool WiiRemote::WriteReport(const unsigned char* buffer)
{
	if (bUseHIDWrite)
	{
		if (!hidWriteThread)
		{
			hidWriteThread = new std::thread(HIDWriteThreadFunction, this);
			if (!hidWriteThread)
				return false;
		}
#ifdef USE_DYNAMIC_HIDQUEUE
		auto lock = new std::lock_guard<std::mutex>(hidWriteQueueLock);
		auto bufferCopy = new unsigned char[ReportLength];
#else
		if (!hid.Queue && !hid.Allocate())
			return false;
		auto lock = new std::lock_guard<std::mutex>(hidWriteQueueLock);
		auto bufferCopy = new unsigned char[ReportLength];
#endif
		memcpy_s(bufferCopy, ReportLength, buffer, ReportLength);
#ifdef USE_DYNAMIC_HIDQUEUE
		hidWriteQueue.push(bufferCopy);
#else
		hid.WriteIndex++;
		hid.WriteIndex &= (hid.MaxQueueEntries - 1);
#endif
		delete lock;
		return true;
	}
	unsigned long written;
	if (!WriteFile(handle, buffer, ReportLength, &written, &overlapped))
	{
		auto error = GetLastError();
		if (error != ERROR_IO_PENDING)
		{
			if (IsConnected())
				bConnectionLost = true;
#ifndef USE_DYNAMIC_HIDQUEUE
			hid.Deallocate();
#endif
			return false;
		}
	}
	return true;
}

bool WiiRemote::StartSampleThread()
{
	if (sampleThread)
		return true;
	sampleThread = new std::thread(SampleStreamThreadFunction, this);
	if (!sampleThread)
	{
		MuteSpeaker(true);
		EnableSpeaker(false);
		return false;
	}
	return true;
}

bool WiiRemote::ReadAddress(const int address, const short size)
{
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::ReadMemory);
	buffer[1] = static_cast<unsigned char>(((address & 0xff000000) >> 24) | GetRumbleBit());
	buffer[2] = static_cast<unsigned char>((address & 0x00ff0000) >> 16);
	buffer[3] = static_cast<unsigned char>((address & 0x0000ff00) >> 8);
	buffer[4] = static_cast<unsigned char>((address & 0x000000ff));
	buffer[5] = static_cast<unsigned char>((size & 0xff00) >> 8);
	buffer[6] = static_cast<unsigned char>((size & 0x00ff));
	return WriteReport(buffer);
}

void WiiRemote::WriteData(const int address, const unsigned char size, const unsigned char* buffer)
{
	unsigned char write[ReportLength] = { 0 };
	write[0] = static_cast<unsigned char>(OutputReport::WriteMemory);
	write[1] = static_cast<unsigned char>(((address & 0xff000000) >> 24) | GetRumbleBit());
	write[2] = static_cast<unsigned char>((address & 0x00ff0000) >> 16);
	write[3] = static_cast<unsigned char>((address & 0x0000ff00) >> 8);
	write[4] = static_cast<unsigned char>((address & 0x000000ff));
	write[5] = size;
	memcpy_s(write + 6, size, buffer, size);
	WriteReport(write);
}

void WiiRemote::OnReadData(const unsigned long bytesRead)
{
	unsigned char buffer[ReportLength];
	memcpy_s(buffer, bytesRead, readBuffer, bytesRead);
	BeginAsyncRead();
	ParseInput(buffer);
}

void WiiRemote::ReadCalibration()
{
	ReadAddress(static_cast<int>(RegisterDevice::Calibration), 7);
}

float WiiRemote::GetBalanceValue(const short sensor, const short minimum, const short middle, const short maximum)
{
	if (maximum == middle || middle == minimum)
		return 0.0f;
	const auto value = (sensor < middle) ?
		68.0f * (static_cast<float>(sensor - minimum) / static_cast<float>(middle - minimum)) :
		68.0f * (static_cast<float>(sensor - middle) / static_cast<float>(maximum - middle)) + 68.0f;
	return value * 0.25f;
}

void WiiRemote::EnableIR(const IRMode mode)
{
	internal.IR.Mode = mode;
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::IR);
	buffer[1] = 0x04 | GetRumbleBit();
	WriteReport(buffer);
	memset(buffer, 0, ReportLength);
	buffer[0] = static_cast<unsigned char>(OutputReport::IR2);
	buffer[1] = 0x04 | GetRumbleBit();
	WriteReport(buffer);
	static unsigned char irSens1[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0xc0 };
	static unsigned char irSens2[] = { 0x40, 0x00 };
	WriteData(static_cast<int>(RegisterDevice::IR), 0x08);
	Sleep(25);
	WriteData(static_cast<int>(RegisterDevice::IRSensitivity1), sizeof(irSens1), irSens1);
	WriteData(static_cast<int>(RegisterDevice::IRSensitivity2), sizeof(irSens2), irSens2);
	WriteData(static_cast<int>(RegisterDevice::IRMode), static_cast<unsigned char>(mode));
}

void WiiRemote::DisableIR()
{
	internal.IR.Mode = IRMode::Off;
	unsigned char buffer[ReportLength] = { 0 };
	buffer[0] = static_cast<unsigned char>(OutputReport::IR);
	buffer[1] = GetRumbleBit();
	WriteReport(buffer);
	memset(buffer, 0, ReportLength);
	buffer[0] = static_cast<unsigned char>(OutputReport::IR2);
	buffer[1] = GetRumbleBit();
	WriteReport(buffer);
}

void WiiRemote::DetectMotionPlusExtensionAsync()
{
	motionPlusDetectCount++;
	ReadAddress(static_cast<int>(RegisterDevice::MotionPlusDetect), 6);
}
