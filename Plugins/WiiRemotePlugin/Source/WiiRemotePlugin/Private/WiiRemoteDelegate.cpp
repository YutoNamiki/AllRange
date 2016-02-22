#include "WiiRemotePluginPrivatePCH.h"
#include "IWiiRemotePlugin.h"
#include "WiiRemoteDelegate.h"
#include "WiiRemoteController.h"

DEFINE_LOG_CATEGORY(WiiRemotePluginLog);

UWiiRemoteDelegate::UWiiRemoteDelegate(const FObjectInitializer& objectInitializer) : Super(objectInitializer) { }

void IWiiRemoteDelegate::SetInterfaceDelegate(UObject* newDelegate)
{
	UE_LOG(LogClass, Log, TEXT("InterfaceDelegate passed: %s"), *newDelegate->GetName());
	if (newDelegate != nullptr)
		InterfaceDelegate = newDelegate;
	else
	{
		if (ValidSelfPointer != nullptr)
			InterfaceDelegate = Cast<UObject>(this);
		else
			InterfaceDelegate = nullptr;
	}
}

bool IWiiRemoteDelegate::IsValidDelegate()
{
	return (InterfaceDelegate != nullptr);
}

UWiiRemoteController* IWiiRemoteDelegate::InternalAddController(int32 newId)
{
	auto validUObject = Cast<UObject>(ValidSelfPointer);
	UWiiRemoteController* controller;
	if (validUObject)
		controller = NewObject<UWiiRemoteController>(validUObject);
	else
		controller = NewObject<UWiiRemoteController>();
	LatestFrame.Add(controller);
	controller->wiiRemoteDelegate = this;
	controller->WiiRemoteID = newId;
	return controller;
}

UWiiRemoteController* IWiiRemoteDelegate::InternalControllerForId(int32 newId)
{
	if (IsWiiRemoteValidId(newId))
		return LatestFrame[newId - 1];
	return nullptr;
}

void IWiiRemoteDelegate::OnConnectedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnConnectionLostFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnBatteryChangedFunction(int32 wiiRemoteId, int32 batteryPercent) { }
void IWiiRemoteDelegate::OnBatteryDrainedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnLEDsChangedFunction(int32 wiiRemoteId, WiiRemoteLED led) { }
void IWiiRemoteDelegate::OnButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteButtons buttons) { }
void IWiiRemoteDelegate::OnAccelChangedFunction(int32 wiiRemoteId, FVector accel) { }
void IWiiRemoteDelegate::OnOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll) { }
void IWiiRemoteDelegate::OnIRChangedFunction(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots) { }
void IWiiRemoteDelegate::OnNunchukConnectedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnNunchukButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons) { }
void IWiiRemoteDelegate::OnNunchukAccelChangedFunction(int32 wiiRemoteId, FVector accel) { }
void IWiiRemoteDelegate::OnNunchukOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll) { }
void IWiiRemoteDelegate::OnNunchukJoystickChangedFunction(int32 wiiRemoteId, float x, float y) { }
void IWiiRemoteDelegate::OnClassicConnectedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnClassicButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons) { }
void IWiiRemoteDelegate::OnClassicJoystickLChangedFunction(int32 wiiRemoteId, float x, float y) { }
void IWiiRemoteDelegate::OnClassicJoystickRChangedFunction(int32 wiiRemoteId, float x, float y) { }
void IWiiRemoteDelegate::OnClassicTriggersChangedFunction(int32 wiiRemoteId, float left, float right) { }
void IWiiRemoteDelegate::OnBalanceConnectedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnBalanceWeightChangedFunction(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard) { }
void IWiiRemoteDelegate::OnMotionPlusDetectedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnMotionPlusEnabledFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnMotionPlusSpeedChangedFunction(int32 wiiRemoteId, FRotator speed) { }
void IWiiRemoteDelegate::OnMotionPlusExtensionConnectedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnMotionPlusExtensionDisconnectedFunction(int32 wiiRemoteId) { }
void IWiiRemoteDelegate::OnExtensionDisconnectedFunction(int32 wiiRemoteId) { }

void IWiiRemoteDelegate::SetReportType(int32 playerIndex, ReportType reportType)
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().SetReportType(playerIndex, reportType);
}

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
	if (!IsValidDelegate())
		return false;
	if (IWiiRemotePlugin::IsAvailable())
		return IWiiRemotePlugin::Get().IsValidDeviceId(wiiRemoteId);
	return false;
}

int32 IWiiRemoteDelegate::WiiRemoteMaxId()
{
	auto id = 0;
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().MaxWiiRemoteId(id);
	return id;
}

void IWiiRemoteDelegate::Startup()
{
	if (IWiiRemotePlugin::IsAvailable())
		IWiiRemotePlugin::Get().SetDelegate(this);
	for (int i = 0; i < WiiRemoteMaxId(); i++)
		InternalAddController(i + 1);
	auto validUObject = Cast<UObject>(ValidSelfPointer);
	if (!InterfaceDelegate && validUObject)
		SetInterfaceDelegate(validUObject);
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
	for (auto i = 0; i < WiiRemoteMaxId(); i++)
	{
		auto controller = LatestFrame[i];
		controller->SetFromWiiRemoteDevciceData(IWiiRemoteDelegate::WiiRemoteLatestData(i + 1));
		controller->WiiRemoteID = i + 1;
	}
}
