#include "WiiRemotePluginPrivatePCH.h"
#include "FWiiRemotePlugin.h"
#include "WiiRemoteDelegate.h"
#include "WiiRemoteManager.h"

static const FString PluginVersion = "ver.SgnmkOriginal";

const FKey EWiiRemoteKeys::A("WiiRemote A");
const FKey EWiiRemoteKeys::B("WiiRemote B");
const FKey EWiiRemoteKeys::Plus("WiiRemote Plus");
const FKey EWiiRemoteKeys::Home("WiiRemote Home");
const FKey EWiiRemoteKeys::Minus("WiiRemote Minus");
const FKey EWiiRemoteKeys::One("WiiRemote One");
const FKey EWiiRemoteKeys::Two("WiiRemote Two");
const FKey EWiiRemoteKeys::Up("WiiRemote Up");
const FKey EWiiRemoteKeys::Down("WiiRemote Down");
const FKey EWiiRemoteKeys::Left("WiiRemote Left");
const FKey EWiiRemoteKeys::Right("WiiRemote Right");
const FKey EWiiRemoteKeys::AccelerationX("WiiRemote AccelerationX");
const FKey EWiiRemoteKeys::AccelerationY("WiiRemote AccelerationY");
const FKey EWiiRemoteKeys::AccelerationZ("WiiRemote AccelerationZ");
const FKey EWiiRemoteKeys::OrientationPitch("WiiRemote OrientationPitch");
const FKey EWiiRemoteKeys::OrientationRoll("WiiRemote OrientationRoll");
const FKey EWiiRemoteKeys::Dot1_X("WiiRemote Dot1 X");
const FKey EWiiRemoteKeys::Dot1_Y("WiiRemote Dot1 Y");
const FKey EWiiRemoteKeys::Dot2_X("WiiRemote Dot2 X");
const FKey EWiiRemoteKeys::Dot2_Y("WiiRemote Dot2 Y");
const FKey EWiiRemoteKeys::Dot3_X("WiiRemote Dot3 X");
const FKey EWiiRemoteKeys::Dot3_Y("WiiRemote Dot3 Y");
const FKey EWiiRemoteKeys::Dot4_X("WiiRemote Dot4 X");
const FKey EWiiRemoteKeys::Dot4_Y("WiiRemote Dot4 Y");
const FKey EWiiRemoteNunchukKeys::C("WiiRemote Nunchuk C");
const FKey EWiiRemoteNunchukKeys::Z("WiiRemote Nunchuk Z");
const FKey EWiiRemoteNunchukKeys::AccelerationX("WiiRemote Nunchuk AccelerationX");
const FKey EWiiRemoteNunchukKeys::AccelerationY("WiiRemote Nunchuk AccelerationY");
const FKey EWiiRemoteNunchukKeys::AccelerationZ("WiiRemote Nunchuk AccelerationZ");
const FKey EWiiRemoteNunchukKeys::OrientationPitch("WiiRemote Nunchuk OrientationPitch");
const FKey EWiiRemoteNunchukKeys::OrientationRoll("WiiRemote Nunchuk OrientationRoll");
const FKey EWiiRemoteNunchukKeys::JoystickX("WiiRemote Nunchuk JoystickX");
const FKey EWiiRemoteNunchukKeys::JoystickY("WiiRemote Nunchuk JoysitckY");
const FKey EWiiRemoteClassicControllerKeys::A("WiiRemote ClassicController A");
const FKey EWiiRemoteClassicControllerKeys::B("WiiRemote ClassicController B");
const FKey EWiiRemoteClassicControllerKeys::X("WiiRemote ClassicController X");
const FKey EWiiRemoteClassicControllerKeys::Y("WiiRemote ClassicController Y");
const FKey EWiiRemoteClassicControllerKeys::Plus("WiiRemote ClassicController Plus");
const FKey EWiiRemoteClassicControllerKeys::Home("WiiRemote ClassicController Home");
const FKey EWiiRemoteClassicControllerKeys::Minus("WiiRemote ClassicController Minus");
const FKey EWiiRemoteClassicControllerKeys::Up("WiiRemote ClassicController Up");
const FKey EWiiRemoteClassicControllerKeys::Down("WiiRemote ClassicController Down");
const FKey EWiiRemoteClassicControllerKeys::Right("WiiRemote ClassicController Right");
const FKey EWiiRemoteClassicControllerKeys::Left("WiiRemote ClassicController Left");
const FKey EWiiRemoteClassicControllerKeys::ZL("WiiRemote ClassicController ZL");
const FKey EWiiRemoteClassicControllerKeys::ZR("WiiRemote ClassicController ZR");
const FKey EWiiRemoteClassicControllerKeys::TriggerL("WiiRemote ClassicController TriggerL");
const FKey EWiiRemoteClassicControllerKeys::TrigerrR("WiiRemote ClassicController TriggerR");
const FKey EWiiRemoteClassicControllerKeys::LeftJoystickX("WiiRemote ClassicController LeftJoystickX");
const FKey EWiiRemoteClassicControllerKeys::LeftJoystickY("WiiRemote ClassicController LeftJoystickY");
const FKey EWiiRemoteClassicControllerKeys::RightJoystickX("WiiRemote ClassicController RightJoystickX");
const FKey EWiiRemoteClassicControllerKeys::RightJoystickY("WiiRemote ClassicController RightJoystickY");
const FKey EWiiRemoteClassicControllerKeys::LeftTrigger("WiiRemote ClassicController LeftTrigger");
const FKey EWiiRemoteClassicControllerKeys::RightTrigger("WiiRemote ClassicController RightTrigger");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopL("WiiRemote BalanceBoard AtRestKilograms TopL");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopR("WiiRemote BalanceBoard AtRestKilograms TopR");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomL("WiiRemote BalanceBoard AtRestKilograms BottomL");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomR("WiiRemote BalanceBoard AtRestKilograms BottomR");
const FKey EWiiRemoteBalanceBoardKeys::AtRestKilograms_Total("WiiRemote BalanceBoard AtRestKilograms Total");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_TopL("WiiRemote BalanceBoard Kilograms TopL");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_TopR("WiiRemote BalanceBoard Kilograms TopR");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_BottomL("WiiRemote BalanceBoard Kilograms BottomL");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_BottomR("WiiRemote BalanceBoard Kilograms BottomR");
const FKey EWiiRemoteBalanceBoardKeys::Kilograms_Total("WiiRemote BalanceBoard Kilograms Total");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_TopL("WiiRemote BalanceBoard Pounds TopL");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_TopR("WiiRemote BalanceBoard Pounds TopR");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_BottomL("WiiRemote BalanceBoard Pounds BottomL");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_BottomR("WiiRemote BalanceBoard Pounds BottomR");
const FKey EWiiRemoteBalanceBoardKeys::Pounds_Total("WiiRemote BalanceBoard Pounds Total");
const FKey EWiiRemoteMotionPlusKeys::SpeedOrientationPitch("WiiRemote MotionPlus SpeedOrientationPitch");
const FKey EWiiRemoteMotionPlusKeys::SpeedOrientationYaw("WiiRemote MotionPlus SpeedOrientationYaw");
const FKey EWiiRemoteMotionPlusKeys::SpeedOrientationRoll("WiiRemote MotionPlus SpeedOrientationRoll");

IMPLEMENT_MODULE(FWiiRemotePlugin, WiiRemotePlugin)

#define LOCTEXT_NAMESPACE "WiiRemotePlugin"

void FWiiRemotePlugin::StartupModule()
{
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
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::OrientationPitch, LOCTEXT("WiiRemoteOrientationPitch", "WiiRemote Orientation Pitch"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::OrientationRoll, LOCTEXT("WiiRemoteOrientationRoll", "WiiRemote Orientation Roll"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot1_X, LOCTEXT("WiiRemoteDot1_X", "WiiRemote Dot1 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot1_Y, LOCTEXT("WiiRemoteDot1_Y", "WiiRemote Dot1 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot2_X, LOCTEXT("WiiRemoteDot2_X", "WiiRemote Dot2 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot2_Y, LOCTEXT("WiiRemoteDot2_Y", "WiiRemote Dot2 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot3_X, LOCTEXT("WiiRemoteDot3_X", "WiiRemote Dot3 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot3_Y, LOCTEXT("WiiRemoteDot3_Y", "WiiRemote Dot3 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot4_X, LOCTEXT("WiiRemoteDot4_X", "WiiRemote Dot4 X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteKeys::Dot4_Y, LOCTEXT("WiiRemoteDot4_Y", "WiiRemote Dot4 Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
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
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::RightJoystickX, LOCTEXT("WiiRemoteeClassicControllerRightJoystickX", "WiiRemote ClassicController RightJoysitck X"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::RightJoystickY, LOCTEXT("WiiRemoteeClassicControllerRightJoystickY", "WiiRemote ClassicController RightJoysitck Y"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::LeftTrigger, LOCTEXT("WiiRemoteeClassicControllerLeftTrigger", "WiiRemote ClassicController LeftTrigger"), FKeyDetails::FloatAxis, FName("WiiRemote")));
	EKeys::AddKey(FKeyDetails(EWiiRemoteClassicControllerKeys::RightTrigger, LOCTEXT("WiiRemoteeClassicControllerRightTrigger", "WiiRemote ClassicController RightTrigger"), FKeyDetails::FloatAxis, FName("WiiRemote")));
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

void FWiiRemotePlugin::SetRumble(bool on)
{

}

void FWiiRemotePlugin::SetRumbleForAsync(int32 milliseconds)
{

}

void FWiiRemotePlugin::MuteSpeaker(bool on)
{

}

void FWiiRemotePlugin::PlaySquareWave(WiiRemoteSpeakerFrequency frequency, int32 volume)
{

}

void FWiiRemotePlugin::PlaySample(WiiRemoteSpeakerFrequency frequency, int32 volume)
{

}

FWiiRemoteDeviceData* FWiiRemotePlugin::LatestData(int32 wiiRemoteId)
{
	if (!this->IsValidDeviceId(wiiRemoteId))
		return nullptr;
	return &(wiiRemoteManager->Data[wiiRemoteId - 1]);
}

void FWiiRemotePlugin::MaxWiiRemoteId(int32 & wiiRemoteId)
{
	wiiRemoteId = wiiRemoteManager->Data.Num();
}

bool FWiiRemotePlugin::IsValidDeviceId(int32 deviceId)
{
	return !(deviceId < 1 || deviceId > wiiRemoteManager->Data.Num());
}

#undef LOCTEXT_NAMESPACE
