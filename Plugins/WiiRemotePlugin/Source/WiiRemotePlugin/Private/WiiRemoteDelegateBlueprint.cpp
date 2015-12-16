#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteDelegateBlueprint.h"
#include "WiiRemoteController.h"
#include "WiiRemoteInterface.h"

UWiiRemoteDelegateBlueprint::UWiiRemoteDelegateBlueprint(const FObjectInitializer& objectInitializer) : Super(objectInitializer) { }

void IWiiRemoteDelegateBlueprint::SetInterfaceDelegate(UObject* newDelegate)
{
	UE_LOG(LogClass, Log, TEXT("InterfaceDelegate passed: %s"), *newDelegate->GetName());
	if (newDelegate->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
		InterfaceDelegate = newDelegate;
	else
	{
		if (ValidSelfPointer->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			InterfaceDelegate = Cast<UObject>(this);
		else
			InterfaceDelegate = nullptr;
		UE_LOG(LogClass, Log, TEXT("MyoDelegateBlueprint Warning: Delegate is NOT set, did your class implement HydraInterface?"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("MyoDelegateBlueprint Warning: Delegate is NOT set, did your class implement MyoInterface?"));
	}
}

bool IWiiRemoteDelegateBlueprint::IsValidDelegate()
{
	return (InterfaceDelegate != nullptr);
}

UWiiRemoteController * IWiiRemoteDelegateBlueprint::InternalAddController(int32 newId)
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

UWiiRemoteController* IWiiRemoteDelegateBlueprint::InternalControllerForId(int32 newId)
{
	if (IsWiiRemoteValidId(newId))
		return LatestFrame[newId - 1];
	return nullptr;
}

void IWiiRemoteDelegateBlueprint::OnConnected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnCennectionLost(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnCennectionLost(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnBatteryChanged(int32 wiiRemoteId, int32 batteryPercent)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnBatteryChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), batteryPercent);
}

void IWiiRemoteDelegateBlueprint::OnBatteryDrained(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnBatteryDrained(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnLEDsChanged(int32 wiiRemoteId, WiiRemoteLED led)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnLEDsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), led);
}

void IWiiRemoteDelegateBlueprint::OnButtonsChanged(int32 wiiRemoteId, FWiiRemoteButtons buttons)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnButtonsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), buttons);
}

void IWiiRemoteDelegateBlueprint::OnAccelChanged(int32 wiiRemoteId, FVector accel)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnAccelChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), accel);
}

void IWiiRemoteDelegateBlueprint::OnOrientationChanged(int32 wiiRemoteId, float pitch, float roll)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnOrientationChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), pitch, roll);
}

void IWiiRemoteDelegateBlueprint::OnIRChanged(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnIRChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), dots);
}

void IWiiRemoteDelegateBlueprint::OnNunchukConnected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnNunchukConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnNunchukButtonsChanged(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnNunchukButtonsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), buttons);
}

void IWiiRemoteDelegateBlueprint::OnNunchukAccelChanged(int32 wiiRemoteId, FVector accel)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnNunchukAccelChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), accel);
}

void IWiiRemoteDelegateBlueprint::OnNunchukOrientationChanged(int32 wiiRemoteId, float pitch, float roll)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnNunchukOrientationChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), pitch, roll);
}

void IWiiRemoteDelegateBlueprint::OnNunchukJoystickChanged(int32 wiiRemoteId, float x, float y)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnNunchukJoystickChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), x, y);
}

void IWiiRemoteDelegateBlueprint::OnClassicConnected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnClassicConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnClassicButtonsChanged(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnClassicButtonsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), buttons);
}

void IWiiRemoteDelegateBlueprint::OnClassicJoystickLChanged(int32 wiiRemoteId, float x, float y)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnClassicJoystickLChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), x, y);
}

void IWiiRemoteDelegateBlueprint::OnClassicJoystickRChanged(int32 wiiRemoteId, float x, float y)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnClassicJoystickRChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), x, y);
}

void IWiiRemoteDelegateBlueprint::OnClassicTriggersChanged(int32 wiiRemoteId, float left, float right)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnClassicTriggersChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), left, right);
}

void IWiiRemoteDelegateBlueprint::OnBalanceConnected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnBalanceConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnBalanceWeightChanged(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnBalanceWeightChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), balanceBoard);
}

void IWiiRemoteDelegateBlueprint::OnMotionPlusDetected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnMotionPlusDetected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnMotionPlusEnabled(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnMotionPlusEnabled(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnMotionPlusSpeedChanged(int32 wiiRemoteId, FRotator speed)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnMotionPlusSpeedChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), speed);
}

void IWiiRemoteDelegateBlueprint::OnMotionPlusExtensionConnected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnMotionPlusExtensionConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnMotionPlusExtensionDisconnected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnMotionPlusExtensionDisconnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::OnExtensionDisconnected(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
		IWiiRemoteInterface::Execute_OnExtensionDisconnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::SetLED(WiiRemoteLED ledBits)
{

}

void IWiiRemoteDelegateBlueprint::SetRumble(bool on)
{

}

void IWiiRemoteDelegateBlueprint::SetRumbleForAsync(int32 milliseconds)
{

}

void IWiiRemoteDelegateBlueprint::MuteSpeaker(bool on)
{

}

void IWiiRemoteDelegateBlueprint::PlaySquareWave(WiiRemoteSpeakerFrequency frequency, int32 volume)
{

}

void IWiiRemoteDelegateBlueprint::PlaySample(WiiRemoteSpeakerFrequency frequency, int32 volume)
{

}

bool IWiiRemoteDelegateBlueprint::IsWiiRemoteValidId(int32 wiiRemoteId)
{
	return (IsValidDelegate() && IWiiRemoteDelegateBlueprint::IsWiiRemoteValidId(wiiRemoteId));
}

void IWiiRemoteDelegateBlueprint::Startup()
{
	IWiiRemoteDelegate::Startup();
	for (int i = 0; i < WiiRemoteMaxId(); i++)
	{
		InternalAddController(i + 1);
	}
	auto validUObject = Cast<UObject>(ValidSelfPointer);
	if (!InterfaceDelegate && validUObject)
		SetInterfaceDelegate(validUObject);
}

void IWiiRemoteDelegateBlueprint::Shutdown()
{
	IWiiRemoteDelegate::Shutdown();
}

void IWiiRemoteDelegateBlueprint::Tick(float deltaTime)
{
	IWiiRemoteDelegate::Tick(deltaTime);
	for (auto i = 0; i < WiiRemoteMaxId(); i++)
	{
		auto controller = LatestFrame[i];
		controller->SetFromWiiRemoteDevciceData(IWiiRemoteDelegate::WiiRemoteLatestData(i + 1));
		controller->WiiRemoteID = i + 1;
	}
}
