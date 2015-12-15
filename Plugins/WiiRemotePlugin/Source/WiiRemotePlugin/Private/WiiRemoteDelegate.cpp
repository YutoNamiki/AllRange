#include "WiiRemotePluginPrivatePCH.h"
#include "IWiiRemotePlugin.h"
#include "WiiRemoteDelegate.h"

DEFINE_LOG_CATEGORY(WiiRemotePluginLog);

UWiiRemoteDelegate::UWiiRemoteDelegate(const FObjectInitializer& objectInitializer) : Super(objectInitializer) { }
void IWiiRemoteDelegate::OnConnected(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnCennectionLost(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnBatteryChanged(int32 wiiRemoteId, int32 batteryPercent) { }
void IWiiRemoteDelegate::OnBatteryDrained(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnLEDsChanged(int32 wiiRemoteId, WiiRemoteLED led) { }
void IWiiRemoteDelegate::OnButtonsChanged(int32 wiiRemoteId, FWiiRemoteButtons buttons) { }
void IWiiRemoteDelegate::OnAccelChanged(int32 wiiRemoteId, FVector accel) { }
void IWiiRemoteDelegate::OnOrientationChanged(int32 wiiRemoteId, float pitch, float roll) { }
void IWiiRemoteDelegate::OnIRChanged(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots) { }
void IWiiRemoteDelegate::OnNunchukConnected(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnNunchukButtonsChanged(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons) { }
void IWiiRemoteDelegate::OnNunchukAccelChanged(int32 wiiRemoteId, FVector accel) { }
void IWiiRemoteDelegate::OnNunchukOrientationChanged(int32 wiiRemoteId, float pitch, float roll) { }
void IWiiRemoteDelegate::OnNunchukJoystickChanged(int32 wiiRemoteId, float x, float y) { }
void IWiiRemoteDelegate::OnClassicConnected(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnClassicButtonsChanged(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons) { }
void IWiiRemoteDelegate::OnClassicJoystickLChanged(int32 wiiRemoteId, float x, float y) { }
void IWiiRemoteDelegate::OnClassicJoystickRChanged(int32 wiiRemoteId, float x, float y) { }
void IWiiRemoteDelegate::OnClassicTriggersChanged(int32 wiiRemoteId, float left, float right) { }
void IWiiRemoteDelegate::OnBalanceConnected(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnBalanceWeightChanged(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard) { }
void IWiiRemoteDelegate::OnMotionPlusDetected(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnMotionPlusEnabled(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnMotionPlusSpeedChanged(int32 wiiRemoteId, FRotator speed) { }
void IWiiRemoteDelegate::OnMotionPlusExtensionConnected(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnMotionPlusExtensionDisconnected(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnExtensionDisconnected(int32 wiiRemoteId) { }

void IWiiRemoteDelegate::SetLED(unsigned char ledBits)
{

}

void IWiiRemoteDelegate::SetRumble(bool on)
{

}

void IWiiRemoteDelegate::SetRumbleForAsync(int32 milliseconds)
{

}

void IWiiRemoteDelegate::MuteSpeaker(bool on)
{

}

void IWiiRemoteDelegate::PlaySquareWave(WiiRemoteSpeakerFrequency frequency, int32 volume)
{

}

void IWiiRemoteDelegate::PlaySample(WiiRemoteSpeakerFrequency frequency, int32 volume)
{

}

void IWiiRemoteDelegate::Tick(float deltaTime)
{

}
