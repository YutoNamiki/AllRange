#include "WiiRemotePluginPrivatePCH.h"
#include "FWiiRemotePlugin.h"
#include "WiiRemoteDelegate.h"
#include "WiiRemoteManager.h"

static const FString PluginVersion = "ver.SgnmkOriginal";

const FKey EWiiRemoteKeys::A("WiiRemoteA");
const FKey EWiiRemoteKeys::B("WiiRemoteB");
const FKey EWiiRemoteKeys::Plus("WiiRemotePlus");
const FKey EWiiRemoteKeys::Home("WiiRemoteHome");
const FKey EWiiRemoteKeys::Minus("WiiRemoteMinus");
const FKey EWiiRemoteKeys::One("WiiRemoteOne");
const FKey EWiiRemoteKeys::Two("WiiRemoteTwo");
const FKey EWiiRemoteKeys::Up("WiiRemoteUp");
const FKey EWiiRemoteKeys::Down("WiiRemoteDown");
const FKey EWiiRemoteKeys::Left("WiiRemoteLeft");
const FKey EWiiRemoteKeys::Right("WiiRemoteRight");
const FKey EWiiRemoteKeys::AccelerationX("WiiRemoteAccelerationX");
const FKey EWiiRemoteKeys::AccelerationY("WiiRemoteAccelerationY");
const FKey EWiiRemoteKeys::AccelerationZ("WiiRemoteAccelerationZ");
const FKey EWiiRemoteKeys::OrientationPitch("WiiRemoteOrientationPitch");
const FKey EWiiRemoteKeys::OrientationRoll("WiiRemoteOrientationRoll");
const FKey EWiiRemoteKeys::Dot1_X("WiiRemoteDot1X");
const FKey EWiiRemoteKeys::Dot1_Y("WiiRemoteDot1Y");
const FKey EWiiRemoteKeys::Dot2_X("WiiRemoteDot2X");
const FKey EWiiRemoteKeys::Dot2_Y("WiiRemoteDot2Y");
const FKey EWiiRemoteKeys::Dot3_X("WiiRemoteDot3X");
const FKey EWiiRemoteKeys::Dot3_Y("WiiRemoteDot3Y");
const FKey EWiiRemoteKeys::Dot4_X("WiiRemoteDot4X");
const FKey EWiiRemoteKeys::Dot4_Y("WiiRemoteDot4Y");
const FKey EWiiRemoteNunchukKeys::C("WiiRemoteNunchukC");
const FKey EWiiRemoteNunchukKeys::Z("WiiRemoteNunchukZ");
const FKey EWiiRemoteNunchukKeys::AccelerationX("WiiRemoteNunchukAccelerationX");
const FKey EWiiRemoteNunchukKeys::AccelerationY("WiiRemoteNunchukAccelerationY");
const FKey EWiiRemoteNunchukKeys::AccelerationZ("WiiRemoteNunchukAccelerationZ");
const FKey EWiiRemoteNunchukKeys::OrientationPitch("WiiRemoteNunchukOrientationPitch");
const FKey EWiiRemoteNunchukKeys::OrientationRoll("WiiRemoteNunchukOrientationRoll");
const FKey EWiiRemoteNunchukKeys::JoystickX("WiiRemoteNunchukJoystickX");
const FKey EWiiRemoteNunchukKeys::JoystickY("WiiRemoteNunchukJoysitckY");
const FKey EWiiRemoteClassicControllerKeys::A("WiiRemoteClassicControllerA");
const FKey EWiiRemoteClassicControllerKeys::B("WiiRemoteClassicControllerB");
const FKey EWiiRemoteClassicControllerKeys::X("WiiRemoteClassicControllerX");
const FKey EWiiRemoteClassicControllerKeys::Y("WiiRemoteClassicControllerY");
const FKey EWiiRemoteClassicControllerKeys::Plus("WiiRemoteClassicControllerPlus");
const FKey EWiiRemoteClassicControllerKeys::Home("WiiRemoteClassicControllerHome");
const FKey EWiiRemoteClassicControllerKeys::Minus("WiiRemoteClassicControllerMinus");
const FKey EWiiRemoteClassicControllerKeys::Up("WiiRemoteClassicControllerUp");
const FKey EWiiRemoteClassicControllerKeys::Down("WiiRemoteClassicControllerDown");
const FKey EWiiRemoteClassicControllerKeys::Right("WiiRemoteClassicControllerRight");
const FKey EWiiRemoteClassicControllerKeys::Left("WiiRemoteClassicControllerLeft");
const FKey EWiiRemoteClassicControllerKeys::ZL("WiiRemoteClassicControllerZL");
const FKey EWiiRemoteClassicControllerKeys::ZR("WiiRemoteClassicControllerZR");
const FKey EWiiRemoteClassicControllerKeys::TriggerL("WiiRemoteClassicControllerTriggerL");
const FKey EWiiRemoteClassicControllerKeys::TrigerrR("WiiRemoteClassicControllerTriggerR");
const FKey EWiiRemoteClassicControllerKeys::LeftJoystickX("WiiRemoteClassicControllerLeftJoystickX");
const FKey EWiiRemoteClassicControllerKeys::LeftJoystickY("WiiRemoteClassicControllerLeftJoystickY");
const FKey EWiiRemoteClassicControllerKeys::RightJoystickX("WiiRemoteClassicControllerRightJoystickX");
const FKey EWiiRemoteClassicControllerKeys::RightJoystickY("WiiRemoteClassicControllerRightJoystickY");
const FKey EWiiRemoteClassicControllerKeys::LeftTrigger("WiiRemoteClassicControllerLeftTrigger");
const FKey EWiiRemoteClassicControllerKeys::RightTrigger("WiiRemoteClassicControllerRightTrigger");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopL("WiiRemoteBalanceBoardAtRestKilogramsTopL");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopR("WiiRemoteBalanceBoardAtRestKilogramsTopR");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomL("WiiRemoteBalanceBoardAtRestKilogramsBottomL");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomR("WiiRemoteBalanceBoardAtRestKilogramsBottomR");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_Total("WiiRemoteBalanceBoardAtRestKilogramsTotal");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_TopL("WiiRemoteBalanceBoardKilogramsTopL");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_TopR("WiiRemoteBalanceBoardKilogramsTopR");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_BottomL("WiiRemoteBalanceBoardKilogramsBottomL");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_BottomR("WiiRemoteBalanceBoardKilogramsBottomR");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_Total("WiiRemoteBalanceBoardKilogramsTotal");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_TopL("WiiRemoteBalanceBoardPoundsTopL");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_TopR("WiiRemoteBalanceBoardPoundsTopR");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_BottomL("WiiRemoteBalanceBoardPoundsBottomL");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_BottomR("WiiRemoteBalanceBoardPoundsBottomR");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_Total("WiiRemoteBalanceBoardPoundsTotal");
const FKey EWiiRemoteMotionPlusKeys::SpeedOrientationPitch("WiiRemoteMotionPlusSpeedOrientationPitch");
const FKey EWiiRemoteMotionPlusKeys::SpeedOrientationYaw("WiiRemoteMotionPlusSpeedOrientationYaw");
const FKey EWiiRemoteMotionPlusKeys::SpeedOrientationRoll("WiiRemoteMotionPlusSpeedOrientationRoll");

IMPLEMENT_MODULE(FWiiRemotePlugin, WiiRemotePlugin)

#define LOCTEXT_NAMESPACE "WiiRemotePlugin"

void FWiiRemotePlugin::StartupModule()
{
	UE_LOG(WiiRemotePluginLog, Warning, TEXT("Startup WiiRemote Plugin Module."));
	UE_LOG(WiiRemotePluginLog, Log, TEXT("Using WiiRemote Plugin verison %s"), *PluginVersion);
	wiiRemoteManager = NewObject<UWiiRemoteManager>();
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::A, LOCTEXT("WiiRemoteA", "WiiRemote A"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::B, LOCTEXT("WiiRemoteB", "WiiRemote B"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::One, LOCTEXT("WiiRemoteOne", "WiiRemote One"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Two, LOCTEXT("WiiRemoteTwo", "WiiRemote Two"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Plus, LOCTEXT("WiiRemotePlus", "WiiRemote Plus"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Home , LOCTEXT("WiiRemoteHome", "WiiRemote Home"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Minus, LOCTEXT("WiiRemoteMinus", "WiiRemote Minus"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Up, LOCTEXT("WiiRemoteUp", "WiiRemote Up"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Down, LOCTEXT("WiiRemoteDown", "WiiRemote Down"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Right, LOCTEXT("WiiRemoteRight", "WiiRemote Right"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Left, LOCTEXT("WiiRemoteLeft", "WiiRemote Left"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::AccelerationX, LOCTEXT("WiiRemoteAccelerationX", "WiiRemote Acceleration X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::AccelerationY, LOCTEXT("WiiRemoteAccelerationY", "WiiRemote Acceleration Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::AccelerationZ, LOCTEXT("WiiRemoteAccelerationZ", "WiiRemote Acceleration Z"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::OrientationPitch, LOCTEXT("WiiRemote OrientationPitch", "WiiRemote Orientation Pitch"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::OrientationRoll, LOCTEXT("WiiRemote OrientationRoll", "WiiRemote Orientation Roll"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot1_X, LOCTEXT("WiiRemoteDot1X", "WiiRemote Dot1 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot1_Y, LOCTEXT("WiiRemoteDot1Y", "WiiRemote Dot1 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot2_X, LOCTEXT("WiiRemoteDot2X", "WiiRemote Dot2 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot2_Y, LOCTEXT("WiiRemoteDot2Y", "WiiRemote Dot2 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot3_X, LOCTEXT("WiiRemoteDot3X", "WiiRemote Dot3 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot3_Y, LOCTEXT("WiiRemoteDot3Y", "WiiRemote Dot3 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot4_X, LOCTEXT("WiiRemoteDot4X", "WiiRemote Dot4 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot4_Y, LOCTEXT("WiiRemoteDot4Y", "WiiRemote Dot4 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::C, LOCTEXT("WiiRemoteNunchukC", "WiiRemote Nunchuk C"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::Z, LOCTEXT("WiiRemoteNunchukZ", "WiiRemote Nunchuk Z"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::AccelerationX, LOCTEXT("WiiRemoteNunchukAccelerationX", "WiiRemote Nunchuk Acceleration X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::AccelerationY, LOCTEXT("WiiRemoteNunchukAccelerationY", "WiiRemote Nunchuk Acceleration Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::AccelerationZ, LOCTEXT("WiiRemoteNunchukAccelerationZ", "WiiRemote Nunchuk Acceleration Z"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::OrientationPitch, LOCTEXT("WiiRemoteNunchukOrientationPitch", "WiiRemote Nunchuk Orientation Pitch"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::OrientationRoll, LOCTEXT("WiiRemoteNunchukOrientationRoll", "WiiRemote Nunchuk Orientation Roll"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::JoystickX, LOCTEXT("WiiRemoteNunchukJoystickX", "WiiRemote Nunchuk Joysitck X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteNunchukKeys::JoystickY, LOCTEXT("WiiRemoteNunchukJoystickY", "WiiRemote Nunchuk Joysitck Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::A, LOCTEXT("WiiRemoteClassicControllerA", "WiiRemote ClassicController A"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::B, LOCTEXT("WiiRemoteClassicControllerB", "WiiRemote ClassicController B"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::X, LOCTEXT("WiiRemoteClassicControllerX", "WiiRemote ClassicController X"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Y, LOCTEXT("WiiRemoteClassicControllerY", "WiiRemote ClassicController Y"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Minus, LOCTEXT("WiiRemoteClassicControllerMinus", "WiiRemote ClassicController Minus"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Home, LOCTEXT("WiiRemoteClassicControllerHome", "WiiRemote ClassicController Home"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Plus, LOCTEXT("WiiRemoteClassicControllerPlus", "WiiRemote ClassicController Plus"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Up, LOCTEXT("WiiRemoteClassicControllerUp", "WiiRemote ClassicController Up"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Down , LOCTEXT("WiiRemoteClassicControllerDown", "WiiRemote ClassicController Down"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Right , LOCTEXT("WiiRemoteClassicControllerRight", "WiiRemote ClassicController Right"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::Left, LOCTEXT("WiiRemoteClassicControllerLeft", "WiiRemote ClassicController Left"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::ZL, LOCTEXT("WiiRemoteClassicControllerZL", "WiiRemote ClassicController ZL"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::ZR, LOCTEXT("WiiRemoteClassicControllerZR", "WiiRemote ClassicController ZR"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::TrigerrR, LOCTEXT("WiiRemoteClassicControllerTrigerrR", "WiiRemote ClassicController TrigerrR"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::TriggerL, LOCTEXT("WiiRemoteClassicControllerTriggerL", "WiiRemote ClassicController TriggerL"), FKeyDetails::GamepadKey, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::LeftJoystickX, LOCTEXT("WiiRemoteClassicControllerLeftJoystickX", "WiiRemote ClassicController LeftJoystickX"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::LeftJoystickY, LOCTEXT("WiiRemoteClassicControllerLeftJoystickY", "WiiRemote ClassicController LeftJoystickY"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::RightJoystickX, LOCTEXT("WiiRemoteClassicControllerRightJoystickX", "WiiRemote ClassicController RightJoysitck X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::RightJoystickY, LOCTEXT("WiiRemoteClassicControllerRightJoystickY", "WiiRemote ClassicController RightJoysitck Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::LeftTrigger, LOCTEXT("WiiRemoteClassicControllerLeftTrigger", "WiiRemote ClassicController LeftTrigger"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::RightTrigger, LOCTEXT("WiiRemoteClassicControllerRightTrigger", "WiiRemote ClassicController RightTrigger"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomL, LOCTEXT("WiiRemoteBalanceBoardAtRestKilogramsBottomL", "WiiRemote BalanceBoard AtRestKilograms BottomL"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomR, LOCTEXT("WiiRemoteBalanceBoardAtRestKilogramsBottomR", "WiiRemote BalanceBoard AtRestKilograms BottomR"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopL, LOCTEXT("WiiRemoteBalanceBoardAtRestKilogramsTopL", "WiiRemote BalanceBoard AtRestKilograms TopL"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopR, LOCTEXT("WiiRemoteBalanceBoardAtRestKilogramsTopR", "WiiRemote BalanceBoard AtRestKilograms TopR"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::AtRestKilograms_Total, LOCTEXT("WiiRemoteBalanceBoardAtRestKilogramsTotal", "WiiRemote BalanceBoard AtRestKilograms Total"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Kilograms_BottomL, LOCTEXT("WiiRemoteBalanceBoardKilogramsBottomL", "WiiRemote BalanceBoard Kilograms BottomL"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Kilograms_BottomR, LOCTEXT("WiiRemoteBalanceBoardKilogramsBottomR", "WiiRemote BalanceBoard Kilograms BottomR"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Kilograms_TopL, LOCTEXT("WiiRemoteBalanceBoardKilogramsTopL", "WiiRemote BalanceBoard Kilograms TopL"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Kilograms_TopR, LOCTEXT("WiiRemoteBalanceBoardKilogramsTopR", "WiiRemote BalanceBoard Kilograms TopR"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Kilograms_Total, LOCTEXT("WiiRemoteBalanceBoardKilogramsTotal", "WiiRemote BalanceBoard Kilograms Total"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Pounds_BottomL, LOCTEXT("WiiRemoteBalanceBoardPoundsBottomL", "WiiRemote BalanceBoard Pounds BottomL"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Pounds_BottomR, LOCTEXT("WiiRemoteBalanceBoardPoundsBottomR", "WiiRemote BalanceBoard Pounds BottomR"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Pounds_TopL, LOCTEXT("WiiRemoteBalanceBoardPoundsTopL", "WiiRemote BalanceBoard Pounds TopL"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Pounds_TopR, LOCTEXT("WiiRemoteBalanceBoardPoundsTopR", "WiiRemote BalanceBoard Pounds TopR"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteBalanceBoardKeys::Pounds_Total, LOCTEXT("WiiRemoteBalanceBoardPoundsTotal", "WiiRemote BalanceBoard Pounds Total"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteMotionPlusKeys::SpeedOrientationPitch, LOCTEXT("WiiRemoteMotionPlusSpeedOrientationPitch", "WiiRemote MotionPlus Speed Orientation Pitch"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteMotionPlusKeys::SpeedOrientationYaw, LOCTEXT("WiiRemoteMotionPlusSpeedOrientationYaw", "WiiRemote MotionPlus Speed Orientation Yaw"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteMotionPlusKeys::SpeedOrientationRoll, LOCTEXT("WiiRemoteMotionPlusSpeedOrientationRoll", "WiiRemote MotionPlus Speed Orientation Roll"), FKeyDetails::FloatAxis, FName("WiiRemote")));
}

void FWiiRemotePlugin::ShutdownModule()
{
	wiiRemoteManager->WiiRemoteDelegate = nullptr;
}

void FWiiRemotePlugin::SetDelegate(IWiiRemoteDelegate* newDelegate)
{
	wiiRemoteManager->WiiRemoteDelegate = newDelegate;
	wiiRemoteManager->Startup();
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote Delegate Set (should only be called once per begin play or you have duplicates)."));
}

void FWiiRemotePlugin::RemoveDelgate()
{
	wiiRemoteManager->WiiRemoteDelegate = nullptr;
	wiiRemoteManager->ShutDown();
}

void FWiiRemotePlugin::Tick(float deltaTime)
{
	wiiRemoteManager->Tick(deltaTime);
}

void FWiiRemotePlugin::SetReportType(int32 playerIndex, ReportType reportType)
{
	if (playerIndex < 0 || playerIndex > 4)
		return;
	auto wiiRemote = &wiiRemoteManager->WiiRemotes[playerIndex];
	if (wiiRemote->IsConnected())
	{
		switch (reportType)
		{
		case ReportType::Buttons:
			wiiRemote->SetReportType(InputReport::Buttons);
			break;
		case ReportType::ButtonsAcceleration:
			wiiRemote->SetReportType(InputReport::ButtonsAcceleration);
			break;
		case ReportType::ButtonsAccelerationIR:
			wiiRemote->SetReportType(InputReport::ButtonsAccelerationIR);
			break;
		case ReportType::ButtonsAccelerationExtension:
			wiiRemote->SetReportType(InputReport::ButtonsAccelerationExtension);
			break;
		case ReportType::ButtonsAccelerationIRExtension:
			wiiRemote->SetReportType(InputReport::ButtonsAccelerationIRExtension);
			break;
		case ReportType::ButtonsBalanceBoard:
			wiiRemote->SetReportType(InputReport::ButtonsBalanceBoard);
			break;
		}
	}
}

void FWiiRemotePlugin::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	if (playerIndex < 0 || playerIndex > 4)
		return;
	auto wiiRemote = &wiiRemoteManager->WiiRemotes[playerIndex];
	if (wiiRemote->IsConnected())
		wiiRemote->SetLEDs(static_cast<BYTE>(ledBits));
}

void FWiiRemotePlugin::SetRumble(int32 playerIndex, bool on)
{
	if (playerIndex < 0 || playerIndex > 4)
		return;
	auto wiiRemote = &wiiRemoteManager->WiiRemotes[playerIndex];
	if (wiiRemote->IsConnected())
		wiiRemote->SetRumble(on);
}

void FWiiRemotePlugin::SetRumbleForAsync(int32 playerIndex, int32 milliseconds)
{
	if (playerIndex < 0 || playerIndex > 4)
		return;
	auto wiiRemote = &wiiRemoteManager->WiiRemotes[playerIndex];
	if (wiiRemote->IsConnected())
		wiiRemote->RumbleForAsync(static_cast<unsigned int>(milliseconds));
}

void FWiiRemotePlugin::EnableSpeaker(int32 playerIndex, bool on)
{
	if (playerIndex < 0 || playerIndex > 4)
		return;
	auto wiiRemote = &wiiRemoteManager->WiiRemotes[playerIndex];
	if (wiiRemote->IsConnected())
		wiiRemote->EnableSpeaker(on);
}

void FWiiRemotePlugin::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequency, int32 volume)
{
	if (playerIndex < 0 || playerIndex > 4)
		return;
	auto wiiRemote = &wiiRemoteManager->WiiRemotes[playerIndex];
	if (wiiRemote->IsConnected())
	{
		if (wiiRemote->Speaker.Enabled)
			wiiRemote->PlaySquareWave(static_cast<SpeakerFrequency>(frequency), static_cast<BYTE>(volume));
	}
}

void FWiiRemotePlugin::PlaySample(int32 playerIndex, FWiiRemoteSample* sample, WiiRemoteSpeakerFrequency frequency, int32 volume)
{
	if (playerIndex < 0 || playerIndex > 4)
		return;
	auto wiiRemote = &wiiRemoteManager->WiiRemotes[playerIndex];
	if (wiiRemote->IsConnected())
	{
		if (wiiRemote->Speaker.Enabled)
		{
			WiiRemoteSample sam;
			sam.Frequency = static_cast<SpeakerFrequency>(sample->Frequency);
			sam.Length = sample->Length;
			sam.Samples = sample->Samples.GetData();
			wiiRemote->PlaySample(sam, volume, static_cast<SpeakerFrequency>(frequency));
		}
	}
}

FWiiRemoteDeviceData* FWiiRemotePlugin::LatestData(int32 wiiRemoteId)
{
	if (!this->IsValidDeviceId(wiiRemoteId))
		return nullptr;
	return &(wiiRemoteManager->Data[wiiRemoteId - 1]);
}

void FWiiRemotePlugin::MaxWiiRemoteId(int32& wiiRemoteId)
{
	wiiRemoteId = wiiRemoteManager->Data.Num();
}

bool FWiiRemotePlugin::IsValidDeviceId(int32 deviceId)
{
	return !(deviceId < 1 || deviceId > wiiRemoteManager->Data.Num());
}

#undef LOCTEXT_NAMESPACE
