#include "WiiRemotePluginPrivatePCH.h"
#include "IWiiRemotePlugin.h"
#include "WiiRemoteDelegate.h"

DEFINE_LOG_CATEGORY(WiiRemotePluginLog);

UWiiRemoteDelegate::UWiiRemoteDelegate(const FObjectInitializer& objectInitializer) : Super(objectInitializer) { }
void IWiiRemoteDelegate::OnConnected() { }
void IWiiRemoteDelegate::OnCennectionLost() { }
void IWiiRemoteDelegate::OnBatteryChanged() { }
void IWiiRemoteDelegate::OnBatteryDrained() { }
void IWiiRemoteDelegate::OnLEDsChanged() { }
void IWiiRemoteDelegate::OnButtonsChanged() { }
void IWiiRemoteDelegate::OnAccelChanged() { }
void IWiiRemoteDelegate::OnOrientationChanged() { }
void IWiiRemoteDelegate::OnIRChanged() { }
void IWiiRemoteDelegate::OnWiiRemoteChanged() { }
void IWiiRemoteDelegate::OnNunchukConnected() { }
void IWiiRemoteDelegate::OnNunchukButtonsChanged() { }
void IWiiRemoteDelegate::OnNunchukAccelChanged() { }
void IWiiRemoteDelegate::OnNunchukOrientationChanged() { }
void IWiiRemoteDelegate::OnNunchukJoystickChanged() { }
void IWiiRemoteDelegate::OnNunchukChanged() { }
void IWiiRemoteDelegate::OnClassicConnected() { }
void IWiiRemoteDelegate::OnClassicButtonsChanged() { }
void IWiiRemoteDelegate::OnClassicJoystickLChanged() { }
void IWiiRemoteDelegate::OnClassicJoystickRChanged() { }
void IWiiRemoteDelegate::OnClassicTriggersChanged() { }
void IWiiRemoteDelegate::OnClassicChanged() { }
void IWiiRemoteDelegate::OnBalanceConnected() { }
void IWiiRemoteDelegate::OnBalanceWeightChanged() { }
void IWiiRemoteDelegate::OnBalanceChanged() { }
void IWiiRemoteDelegate::OnMotionPlusDetected() { }
void IWiiRemoteDelegate::OnMotionPlusEnabled() { }
void IWiiRemoteDelegate::OnMotionPlusSpeedChanged() { }
void IWiiRemoteDelegate::OnMotionPlusExtensionConnected() { }
void IWiiRemoteDelegate::OnMotionPlusExtensionDisconnected() { }
void IWiiRemoteDelegate::OnMotionPlusChanged() { }
void IWiiRemoteDelegate::OnExtensionDisconnected() { }
void IWiiRemoteDelegate::OnExtensionPartiallyInserted() { }
void IWiiRemoteDelegate::OnExtensionConnected() { }
void IWiiRemoteDelegate::OnExtensionChanged() { }
void IWiiRemoteDelegate::ChangedAll() { }

void IWiiRemoteDelegate::SetLED(unsigned char ledBits)
{

}

void IWiiRemoteDelegate::SetRumble(bool on)
{

}

void IWiiRemoteDelegate::SetRumbleForAsync(unsigned int milliseconds)
{

}

void IWiiRemoteDelegate::MuteSpeaker(bool on)
{

}

void IWiiRemoteDelegate::PlaySquareWave(SpeakerFrequency frequency, unsigned char volume)
{

}

void IWiiRemoteDelegate::PlaySample(SpeakerFrequency frequency, unsigned char volume)
{

}

void IWiiRemoteDelegate::Tick(float deltaTime)
{

}
