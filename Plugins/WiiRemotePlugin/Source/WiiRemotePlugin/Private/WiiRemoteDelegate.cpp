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

void IWiiRemoteDelegate::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().SetLED(playerIndex, ledBits);
}

void IWiiRemoteDelegate::SetRumble(int32 playerIndex, bool on)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().SetRumble(playerIndex, on);
}

void IWiiRemoteDelegate::SetRumbleForAsync(int32 playerIndex, int32 milliseconds)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().SetRumbleForAsync(playerIndex, milliseconds);
}

void IWiiRemoteDelegate::EnableSpeaker(int32 playerIndex, bool on)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().EnableSpeaker(playerIndex, on);
}

void IWiiRemoteDelegate::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequency, int32 volume)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().PlaySquareWave(playerIndex, frequency, volume);
}

void IWiiRemoteDelegate::PlaySample(int32 playerIndex, FWiiRemoteSample sample, WiiRemoteSpeakerFrequency frequency, int32 volume)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().PlaySample(playerIndex, &sample, frequency, volume);
}

FWiiRemoteDeviceData* IWiiRemoteDelegate::WiiRemoteLatestData(int32 wiiRemoteId)
{
	if (IWiiRemotePlugin::IsAvailable())
		return IWiiRemotePlugin::Get().LatestData(wiiRemoteId);
	return nullptr;
}

bool IWiiRemoteDelegate::IsWiiRemoteValidId(int32 wiiRemoteId)
{
	if (IWiiRemotePlugin::IsAvailable())
		return IWiiRemotePlugin::Get().IsValidDeviceId(wiiRemoteId);
	return false;
}

int32 IWiiRemoteDelegate::WiiRemoteMaxId()
{
	auto size = 0;
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().MaxWiiRemoteId(size);
	return size;
}

void IWiiRemoteDelegate::Startup()
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().SetDelegate(this);
}

void IWiiRemoteDelegate::Shutdown()
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().RemoveDelgate();
}

void IWiiRemoteDelegate::Tick(float deltaTime)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().Tick(deltaTime);
}
