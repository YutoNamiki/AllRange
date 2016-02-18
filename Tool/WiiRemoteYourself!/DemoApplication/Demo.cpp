#pragma comment(lib, "WiiRemoteYourself!.lib")

#include "Demo.h"
#include "WiiRemoteYourself.h"
#include <mmsystem.h>
#include <tchar.h>
#include <assert.h>

#define ON_PRESS_RELEASE(button, pressed_action, released_action)	\
{																	\
	const auto pressed = remote.Buttons.button();					\
	if(pressed)														\
	{				   /* just pressed? */							\
		if(!last##button) pressed_action;							\
	}																\
	else if(last##button) /* just released */						\
		released_action;											\
	/* remember the current button state for next time */			\
	last##button = pressed;										\
}																	

void OnStateChange(WiiRemote& remote, StateChangeFlags changed, const WiiRemoteState& newState);
void PrintTitle(void* console);

DemoMode Connecting();
DemoMode Moving();

int main()
{
	SetConsoleTitle(_T("- WiiRemoteYourself! - Demo: "));
	void* console = GetStdHandle(STD_OUTPUT_HANDLE);
	PrintTitle(console);

	WiiRemoteSample sineSample, daisySample;
	if (!WiiRemote::Load16BitMonoSampleRAW("1kSine16 (3130).raw", true, SpeakerFrequency::Frequency3130Hz, sineSample))
	{
		printf_s("\r  ** can't find 'sine16 (3130).raw' - (sample won't work!) **");
		printf_s("\r");
		printf_s(BLANK_LINE);
	}
	if (!WiiRemote::Load16BitMonoSampleWAV("Daisy16 (3130).wav", daisySample))
	{
		printf_s("\r  ** can't find 'Daisy16 (3130).wav' - (sample won't work!) **");
		printf_s("\r");
		printf_s(BLANK_LINE);
	}
	WiiRemote remote;
	remote.ChangedCallback = OnStateChange;
	remote.CallbackTriggerFlags = static_cast<StateChangeFlags>(
		static_cast<int>(StateChangeFlags::Connected) |
		static_cast<int>(StateChangeFlags::ExtensionChanged) |
		static_cast<int>(StateChangeFlags::MotionPlusChanged));

reconnect:
	COORD position = { 0, 6 };
	static const char* waitString[] = { ".  ", "..  ", "..." };
	unsigned int count = 0;
	SetConsoleCursorPosition(console, position);
	SetConsoleTextAttribute(console, WHITE);
	printf_s("  Looking for a WiiRemote     ");
	while (!remote.Connect())
	{
		printf_s("\b\b\b\b%s ", waitString[count % 3]);
		count++;
	}
	remote.SetLEDs(0x0F);
	SetConsoleTextAttribute(console, BRIGHT_CYAN);
	printf_s("\b\b\b\b... connected!");
	if (remote.IsBalanceBoard())
	{
		SetConsoleTextAttribute(console, BRIGHT_WHITE);
		printf_s("  (Balance Board)");
	}
	Sleep(2000);

	COORD cursorPosition = { 0, 6 };
	printf_s("\n\n");
	WiiRemote* extraMotes[7] = { nullptr };
	auto detected = 0;
	while (detected < 7)
	{
		WiiRemote* next = new WiiRemote();
		if (!next->Connect())
			break;
		extraMotes[detected++] = next;
		SetConsoleTextAttribute(console, WHITE);
		printf_s("    also found WiiRemote ");
		SetConsoleTextAttribute(console, BRIGHT_GREEN);
		if (next->IsBalanceBoard())
		{
			SetConsoleTextAttribute(console, WHITE);
			printf_s("  (Balance Board)");
		}
		printf_s("\n\n");
		Sleep(500);
	}
	SetConsoleTextAttribute(console, WHITE);
	printf_s(((detected == 7) ? "    (can't detect any more)." : "    (no more found.)"));
	Sleep(2000);
	for (auto index = 0; index < detected; index++)
		delete extraMotes[index];
	SetConsoleCursorPosition(console, cursorPosition);
	SetConsoleTextAttribute(console, BRIGHT_WHITE);
	printf_s("\r  -- TRY: B = rumble, A = square, 1 = sine, 2 = daisy, Home = Exit --\n");
	auto lastRumbleTime = timeGetTime();
	auto lastLEDTime = timeGetTime();
	auto RumbleText = true;
	auto litLED = 0;
	while (!remote.Buttons.Home())
	{
		while (remote.RefreshState() == StateChangeFlags::NoChange)
			Sleep(1);
		cursorPosition.Y = 8;
		SetConsoleCursorPosition(console, cursorPosition);
		if (remote.ConnectionLost())
		{
			SetConsoleTextAttribute(console, BRIGHT_RED);
			printf_s("   *** connection lost! ***\n");
			printf_s(BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE
				BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE BLANK_LINE
				BLANK_LINE BLANK_LINE BLANK_LINE);
			Sleep(2000);
			COORD position = { 0, 6 };
			SetConsoleCursorPosition(console, position);
			printf_s(BLANK_LINE BLANK_LINE BLANK_LINE);
			goto reconnect;
		}
		remote.SetRumble(remote.Buttons.B());
		static auto lastMinus = false;
		if (remote.Buttons.Minus() && !lastMinus && ((remote.ExtensionType == ExtensionType::MotionPlus) || (remote.ExtensionType == ExtensionType::WiiRemotePlus)))
			remote.DisableMotionPlus();
		lastMinus = remote.Buttons.Minus();
		static auto lastA = false, lastOne = false, lastTwo = false;
		if (!remote.IsBalanceBoard())
		{
			ON_PRESS_RELEASE(A, remote.PlaySquareWave(SpeakerFrequency::Frequency3130Hz, 0x20), remote.EnableSpeaker(false));
			ON_PRESS_RELEASE(One, remote.PlaySample(sineSample), remote.EnableSpeaker(false));
			ON_PRESS_RELEASE(Two, remote.PlaySample(daisySample), remote.EnableSpeaker(false));
		}
		SetConsoleTextAttribute(console, CYAN);
		printf_s("  Bettery: ");
		remote.bBatteryDrained ?
			SetConsoleTextAttribute(console, BRIGHT_RED) :
			(remote.BatteryPercent >= 30 ? SetConsoleTextAttribute(console, BRIGHT_GREEN) : SetConsoleTextAttribute(console, BRIGHT_YELLOW));
		printf_s("%3u%%  ", remote.BatteryPercent);
		const auto currentTime = timeGetTime();
		if ((currentTime - lastLEDTime) >= 1000)
		{
			remote.SetLEDs(static_cast<unsigned char>(1 << litLED));
			litLED = (++litLED) % 4;
			lastLEDTime = timeGetTime();
		}
		SetConsoleTextAttribute(console, CYAN);
		printf_s("LEDs: ");
		SetConsoleTextAttribute(console, WHITE);
		printf_s("[");
		for (auto led = 0; led < 4; led++)
		{
			if (remote.LED.Lit(led))
			{
				SetConsoleTextAttribute(console, BRIGHT_CYAN);
				printf_s("*");
			}
			else
			{
				SetConsoleTextAttribute(console, WHITE);
				printf_s("-");
			}
		}
		SetConsoleTextAttribute(console, WHITE);
		printf_s("] ");
		if (remote.bRumble)
		{
			SetConsoleTextAttribute(console, BRIGHT_WHITE);
			printf_s(RumbleText ? " RUMBLE" : "RUMBLE ");
			if ((currentTime - lastRumbleTime) >= 110)
			{
				RumbleText = !RumbleText;
				lastRumbleTime = currentTime;
			}
		}
		else
			printf_s("       ");
		SetConsoleTextAttribute(console, CYAN);
		printf_s("      using %s\n", (remote.IsUsingHIDWrites() ? "HID writes" : "WriteFile()"));
		printf_s("      ID: ");
		SetConsoleTextAttribute(console, YELLOW);
		printf_s("%I64u", remote.UniqueID);
		SetConsoleTextAttribute(console, CYAN);
		printf_s("      ID2: ");
		SetConsoleTextAttribute(console, WHITE);
		printf_s("%I64u\n", remote.UniqueID2);
		SetConsoleTextAttribute(console, CYAN);
		printf_s("  Buttons: ");
		SetConsoleTextAttribute(console, WHITE);
		printf_s("[");
		for (auto bit = 0; bit < 16; bit++)
		{
			const auto mask = static_cast<unsigned short>(1 << bit);
			if ((static_cast<unsigned short>(WiiRemoteButtons::Mask::All) & mask) == 0)
				continue;
			const auto buttonName = WiiRemote::GetButtonNameFromBit(bit);
			const auto pressed = ((remote.Buttons.Bits & mask) != 0);
			if (bit > 0)
			{
				SetConsoleTextAttribute(console, CYAN);
				printf_s("|");
			}
			if (pressed)
			{
				SetConsoleTextAttribute(console, BRIGHT_WHITE);
				printf_s("%s", buttonName);
			}
			else
			{
				SetConsoleTextAttribute(console, WHITE);
				printf_s("%*s", static_cast<int>(strlen(buttonName)), "");
			}
		}
		SetConsoleTextAttribute(console, WHITE);
		printf_s("]\n");
		SetConsoleTextAttribute(console, CYAN);
		printf_s("     Accel:");
		remote.IsBalanceBoard() ? SetConsoleTextAttribute(console, RED) : SetConsoleTextAttribute(console, WHITE);
		printf_s("  X %+2.3f  Y %+2.3f  Z %+2.3f  \n", remote.Acceleration.Data.X, remote.Acceleration.Data.Y, remote.Acceleration.Data.Z);
		SetConsoleTextAttribute(console, CYAN);
		printf_s("   Orient:");
		SetConsoleTextAttribute(console, remote.IsBalanceBoard() ? RED : WHITE);
		printf_s("UpdateAge %3u  ", remote.Acceleration.Orientation.UpdateAge);
		if (remote.Acceleration.Orientation.UpdateAge > 10)
			SetConsoleTextAttribute(console, RED);
		printf_s("Pitch: %4ddeg  Roll:%4ddeg  \n                           (X %+.3f  Y %+.3f  Z %+.3f)      \n",
			static_cast<int>(remote.Acceleration.Orientation.Rotation.Pitch), static_cast<int>(remote.Acceleration.Orientation.Rotation.Roll),
			remote.Acceleration.Orientation.Data.X, remote.Acceleration.Orientation.Data.Y, remote.Acceleration.Orientation.Data.Z);
		SetConsoleTextAttribute(console, CYAN);
		printf_s("      IR:");
		remote.IsBalanceBoard() ? SetConsoleTextAttribute(console, RED) : SetConsoleTextAttribute(console, WHITE);
		printf_s("  Mode %s   ", ((remote.IR.Mode == IRMode::Off) ? "OFF  " : ((remote.IR.Mode == IRMode::Basic) ? "BASIC" :
			((remote.IR.Mode == IRMode::Extended) ? "EXT. " : "FULL "))));
		const auto dotSize = (remote.IR.Mode == IRMode::Extended);
		for (auto index = 0; index < 4; index++)
		{
			auto& dot = remote.IR.Dot[index];
			if (!remote.IsBalanceBoard())
				SetConsoleTextAttribute(console, WHITE);
			printf_s("%u: ", index);
			if (dot.Visible)
			{
				SetConsoleTextAttribute(console, WHITE);
				printf_s("Seen       ");
			}
			else
			{
				SetConsoleTextAttribute(console, RED);
				printf_s("Not Seen   ");
			}
			printf_s("Size");
			if (dotSize)
			{
				printf_s("%3d ", dot.Size);
			}
			else
			{
				SetConsoleTextAttribute(console, RED);
				printf_s(" n/a");
				if (dot.Visible)
					SetConsoleTextAttribute(console, WHITE);
			}
			printf_s("  X %.3f  Y %.3f\n", dot.Data.X, dot.Data.Y);
			if (index < 3)
				printf_s("                        ");
		}
		SetConsoleTextAttribute(console, CYAN);
		printf_s("  Speaker:");
		remote.IsBalanceBoard() ? SetConsoleTextAttribute(console, RED) : SetConsoleTextAttribute(console, WHITE);
		printf_s("  %s | %s    ", (remote.Speaker.Enabled ? "On " : "Off"), (remote.Speaker.Muted ? "Muted" : "     "));
		if (!remote.Speaker.Enabled || remote.Speaker.Muted)
			SetConsoleTextAttribute(console, RED);
		else
			SetConsoleTextAttribute(console, WHITE);
		printf_s("Frequency %4u Hz   Volume 0x%02x\n", WiiRemote::GetFrequencyLookup(static_cast<int>(remote.Speaker.Frequency)), remote.Speaker.Volume);
		SetConsoleTextAttribute(console, CYAN);
		printf_s("__________\n  Extnsn.:  ");
		switch (remote.ExtensionType)
		{
		case ExtensionType::None:
			SetConsoleTextAttribute(console, RED);
			printf_s("None\n");
			printf_s(BLANK_LINE BLANK_LINE BLANK_LINE);
			break;
		case ExtensionType::PartiallyInserted:
			SetConsoleTextAttribute(console, BRIGHT_RED);
			printf_s("Partially Inserted\n");
			printf_s(BLANK_LINE BLANK_LINE BLANK_LINE);
			break;
		default:
			if (remote.ExtensionType == ExtensionType::Nunchuk)
			{
				SetConsoleTextAttribute(console, BRIGHT_WHITE);
				printf_s("Nunchuk   ");
				SetConsoleTextAttribute(console, CYAN);
				printf_s("Buttons: ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("[");
				SetConsoleTextAttribute(console, BRIGHT_WHITE);
				printf_s(remote.Nunchuk.C ? "C" : " ");
				SetConsoleTextAttribute(console, CYAN);
				printf_s("|");
				SetConsoleTextAttribute(console, BRIGHT_WHITE);
				printf_s(remote.Nunchuk.Z ? "Z" : " ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("]   ");
				SetConsoleTextAttribute(console, CYAN);
				printf_s("Joystick:  ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("X %+2.3f  Y %+2.3f\n", remote.Nunchuk.Joystick.Data.X, remote.Nunchuk.Joystick.Data.Y);
				SetConsoleTextAttribute(console, CYAN);
				printf_s("     Accel:");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("   X %+2.3f  Y %+2.3f  Z %+2.3f  \n", remote.Nunchuk.Acceleration.Data.X, remote.Nunchuk.Acceleration.Data.Y, remote.Nunchuk.Acceleration.Data.Z);
				SetConsoleTextAttribute(console, CYAN);
				printf_s("   Orient:");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("   UpdateAge %3u  ", remote.Nunchuk.Acceleration.Orientation.UpdateAge);
				if (remote.Nunchuk.Acceleration.Orientation.UpdateAge > 10)
					SetConsoleTextAttribute(console, RED);
				printf_s("Pitch %4ddeg  Roll %4ddeg  \n                           (X %+.2f  Y %+.2f  Z %+.2f)     \n",
					static_cast<int>(remote.Nunchuk.Acceleration.Orientation.Rotation.Pitch), static_cast<int>(remote.Nunchuk.Acceleration.Orientation.Rotation.Roll),
					remote.Nunchuk.Acceleration.Orientation.Data.X, remote.Nunchuk.Acceleration.Orientation.Data.Y, remote.Nunchuk.Acceleration.Orientation.Data.Z);
			}
			if (remote.ExtensionType == ExtensionType::Classic || remote.ExtensionType == ExtensionType::GH3GhwtGuitar || remote.ExtensionType == ExtensionType::GhwtDrums)
			{
				SetConsoleTextAttribute(console, BRIGHT_WHITE);
				if (remote.ExtensionType == ExtensionType::Classic)
					printf_s("Classic Controller   ");
				else if (remote.ExtensionType == ExtensionType::GH3GhwtGuitar)
					printf_s("GH3/GHWT Guitar      ");
				else
					printf_s("GHWT Drums           ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("L:  ");
				SetConsoleTextAttribute(console, CYAN);
				printf_s("Joy ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("X %+2.3f  Y %+2.3f  ", remote.ClassicController.JoystickL.Data.X, remote.ClassicController.JoystickL.Data.Y);
				SetConsoleTextAttribute(console, CYAN);
				printf_s("Trigger ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("%+2.3f\n", remote.ClassicController.TriggerL);
				printf_s("                                 R:  ");
				SetConsoleTextAttribute(console, CYAN);
				printf_s("Joy ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("X %+2.3f  Y %+2.3f  ", remote.ClassicController.JoystickR.Data.X, remote.ClassicController.JoystickR.Data.Y);
				SetConsoleTextAttribute(console, CYAN);
				printf_s("Trigger ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("%+2.3f\n", remote.ClassicController.TriggerR);
				SetConsoleTextAttribute(console, CYAN);
				printf_s("  Buttons: ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("[");
				for (auto bit = 0; bit < 16; bit++)
				{
					const auto mask = static_cast<unsigned short>(1 << bit);
					if ((static_cast<unsigned short>(WiiRemoteButtons::Mask::All) & mask) == 0)
						continue;
					const auto buttonName = WiiRemote::GetClassicButtonNameFromBit(bit);
					const auto seperator = (bit == 0) ? "" : "|";
					const auto pressed = ((static_cast<unsigned short>(remote.ClassicController.Buttons.Bits) & mask) != 0);
					if (pressed)
					{
						SetConsoleTextAttribute(console, BRIGHT_WHITE);
						printf_s("%s", buttonName);
					}
					else
					{
						SetConsoleTextAttribute(console, WHITE);
						printf_s("%*s", static_cast<int>(strlen(buttonName)), "");
					}
				}
				SetConsoleTextAttribute(console, WHITE);
				printf_s("]");
			}
			if (remote.ExtensionType == ExtensionType::BalanceBoard)
			{
				SetConsoleTextAttribute(console, BRIGHT_WHITE);
				printf_s("Balance Board");
				SetConsoleTextAttribute(console, CYAN);
				printf_s("   Weight: ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("TL    %6.2f", remote.BalanceBoard.Kg.TopL);
				SetConsoleTextAttribute(console, CYAN);
				printf_s(" Kg");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("  TR %6.2f", remote.BalanceBoard.Kg.TopR);
				SetConsoleTextAttribute(console, CYAN);
				printf_s(" Kg\n");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("                                    BL    %6.2f", remote.BalanceBoard.Kg.BottomL);
				SetConsoleTextAttribute(console, CYAN);
				printf_s(" Kg");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("  BR %6.2f", remote.BalanceBoard.Kg.BottomR);
				SetConsoleTextAttribute(console, CYAN);
				printf_s(" Kg \n");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("                                    Total %6.2f", remote.BalanceBoard.Kg.Total);
				SetConsoleTextAttribute(console, CYAN);
				printf_s(" Kg");
			}
			if (remote.ExtensionType == ExtensionType::MotionPlus || remote.ExtensionType == ExtensionType::WiiRemotePlus)
			{
				SetConsoleTextAttribute(console, BRIGHT_WHITE);
				printf_s("Motion Plus");
				SetConsoleTextAttribute(console, CYAN);
				printf_s("    Raw: ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("Yaw: %04hx  Pitch: %04hx  Roll: %04hx  \n", remote.MotionPlus.Raw.Yaw, remote.MotionPlus.Raw.Pitch, remote.MotionPlus.Raw.Roll);
				SetConsoleTextAttribute(console, CYAN);
				printf_s("                         Float: ");
				SetConsoleTextAttribute(console, WHITE);
				printf_s("   %8.3fdeg  %8.3fdeg  %8.3fdeg\n", remote.MotionPlus.Speed.Yaw, remote.MotionPlus.Speed.Pitch, remote.MotionPlus.Speed.Roll);
				printf_s(BLANK_LINE BLANK_LINE);
			}
			break;
		}
	}
	remote.Disconnect();
	SetConsoleTextAttribute(console, BRIGHT_WHITE);
	CloseHandle(console);
	return 0;
}

void OnStateChange(WiiRemote& remote, StateChangeFlags changed, const WiiRemoteState& newState)
{
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::Connected))
	{
		if (newState.ExtensionType != ExtensionType::BalanceBoard)
		{
			if (newState.bExtension)
				remote.SetReportType(InputReport::ButtonsAccelerationIRExtension);
			else
				remote.SetReportType(InputReport::ButtonsAccelerationIR);
		}
	}
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusDetected))
		remote.EnableMotionPlus();
	else if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusExtensionConnected))
	{
		if (remote.MotionPlusEnabled())
			remote.EnableMotionPlus();
	}
	else if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusExtensionDisconnected))
	{
		if (remote.MotionPlusConnected())
			remote.EnableMotionPlus();
	}
	else if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ExtensionConnected))
	{
		if (!remote.IsBalanceBoard())
			remote.SetReportType(InputReport::ButtonsAccelerationIRExtension);
	}
	else if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ExtensionDisconnected))
		remote.SetReportType(InputReport::ButtonsAccelerationIR);
}

void PrintTitle(void* console)
{
	SetConsoleTextAttribute(console, BRIGHT_WHITE);
	printf_s("\n");
	printf_s("   -WiiRemoteYourself!- ");
	SetConsoleTextAttribute(console, WHITE);
	printf_s("    library Demo :   ");
	SetConsoleTextAttribute(console, CYAN);
	printf_s(" | (c) ");
	SetConsoleTextAttribute(console, BRIGHT_CYAN);
	printf_s("sgnmk 2016\n\n");
	SetConsoleTextAttribute(console, CYAN);
	printf_s("  v2.0___________________________________________________________\n\n\n");
}

DemoMode Connecting()
{

	return DemoMode();
}

DemoMode Moving()
{

	return DemoMode();
}
