#include "WiiRemotePluginPrivatePCH.h"
#include "FWiiRemotePlugin.h"
#include "WiiRemoteManager.h"

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

}

void FWiiRemotePlugin::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
