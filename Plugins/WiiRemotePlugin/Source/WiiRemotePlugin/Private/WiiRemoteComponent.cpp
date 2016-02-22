// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteComponent.h"
#include "WiiRemoteInterface.h"

UWiiRemoteComponent::UWiiRemoteComponent( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	bWantsInitializeComponent = true;
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UWiiRemoteComponent::OnRegister()
{
	Super::OnRegister();
	ValidSelfPointer = this;
	SetInterfaceDelegate(GetOwner());
	Startup();
}

void UWiiRemoteComponent::OnUnregister()
{
	Super::OnUnregister();
	Shutdown();
}

void UWiiRemoteComponent::BeginPlay()
{
	Super::BeginPlay();
	ValidSelfPointer = this;
	SetInterfaceDelegate(GetOwner());
	Startup();
}

void UWiiRemoteComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Shutdown();
	Super::EndPlay(EndPlayReason);
}

void UWiiRemoteComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);
	IWiiRemoteDelegate::Tick(deltaTime);
}

void UWiiRemoteComponent::SetReportType(int32 playerIndex, ReportType reportType)
{
	IWiiRemoteDelegate::SetReportType(playerIndex, reportType);
}

void UWiiRemoteComponent::SetLED(int32 playerIndex, WiiRemoteLED ledBits)
{
	IWiiRemoteDelegate::SetLED(playerIndex, ledBits);
}

void UWiiRemoteComponent::SetVibrate(int32 playerIndex, bool on)
{
	IWiiRemoteDelegate::SetRumble(playerIndex, on);
}

void UWiiRemoteComponent::SetVibrateForAsync(int32 playerIndex, int32 milliseconds)
{
	IWiiRemoteDelegate::SetRumbleForAsync(playerIndex, milliseconds);
}

void UWiiRemoteComponent::EnableSpeaker(int32 playerIndex, bool on)
{
	IWiiRemoteDelegate::EnableSpeaker(playerIndex, on);
}

void UWiiRemoteComponent::PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequancy, int32 volume)
{
	IWiiRemoteDelegate::PlaySquareWave(playerIndex, frequancy, volume);
}

void UWiiRemoteComponent::OnConnectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnConnected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnConnectionLostFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnConnectionLost.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnConnectionLost(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnBatteryChangedFunction(int32 wiiRemoteId, int32 batteryPercent)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnBatteryChanged.Broadcast(InternalControllerForId(wiiRemoteId), batteryPercent);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnBatteryChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), batteryPercent);
	}
}

void UWiiRemoteComponent::OnBatteryDrainedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnBatteryDrained.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnBatteryDrained(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnLEDsChangedFunction(int32 wiiRemoteId, WiiRemoteLED led)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnLEDsChanged.Broadcast(InternalControllerForId(wiiRemoteId), led);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnLEDsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), led);
	}
}

void UWiiRemoteComponent::OnButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteButtons buttons)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnButtonsChanged.Broadcast(InternalControllerForId(wiiRemoteId), buttons);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnButtonsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), buttons);
	}
}

void UWiiRemoteComponent::OnAccelChangedFunction(int32 wiiRemoteId, FVector accel)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnAccelChanged.Broadcast(InternalControllerForId(wiiRemoteId), accel);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnAccelChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), accel);
	}
}

void UWiiRemoteComponent::OnOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnOrientationChanged.Broadcast(InternalControllerForId(wiiRemoteId), pitch, roll);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnOrientationChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), pitch, roll);
	}
}

void UWiiRemoteComponent::OnIRChangedFunction(int32 wiiRemoteId, TArray<FWiiRemoteDot> dots)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnIRChanged.Broadcast(InternalControllerForId(wiiRemoteId), dots);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnIRChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), dots);
	}
}

void UWiiRemoteComponent::OnNunchukConnectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnNunchukConnected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnNunchukConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnNunchukButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteNunchukButtons buttons)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnNunchukButtonsChanged.Broadcast(InternalControllerForId(wiiRemoteId), buttons);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnNunchukButtonsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), buttons);
	}
}

void UWiiRemoteComponent::OnNunchukAccelChangedFunction(int32 wiiRemoteId, FVector accel)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnNunchukAccelChanged.Broadcast(InternalControllerForId(wiiRemoteId), accel);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnNunchukAccelChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), accel);
	}
}

void UWiiRemoteComponent::OnNunchukOrientationChangedFunction(int32 wiiRemoteId, float pitch, float roll)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnNunchukOrientationChanged.Broadcast(InternalControllerForId(wiiRemoteId), pitch, roll);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnNunchukOrientationChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), pitch, roll);
	}
}

void UWiiRemoteComponent::OnNunchukJoystickChangedFunction(int32 wiiRemoteId, float x, float y)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnNunchukJoystickChanged.Broadcast(InternalControllerForId(wiiRemoteId), x, y);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnNunchukJoystickChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), x, y);
	}
}

void UWiiRemoteComponent::OnClassicConnectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnClassicConnected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnClassicConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnClassicButtonsChangedFunction(int32 wiiRemoteId, FWiiRemoteClassicControllerButtons buttons)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnClassicButtonsChanged.Broadcast(InternalControllerForId(wiiRemoteId), buttons);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnClassicButtonsChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), buttons);
	}
}

void UWiiRemoteComponent::OnClassicJoystickLChangedFunction(int32 wiiRemoteId, float x, float y)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnClassicJoystickLChanged.Broadcast(InternalControllerForId(wiiRemoteId), x , y);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnClassicJoystickLChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), x, y);
	}
}

void UWiiRemoteComponent::OnClassicJoystickRChangedFunction(int32 wiiRemoteId, float x, float y)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnClassicJoystickRChanged.Broadcast(InternalControllerForId(wiiRemoteId), x, y);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnClassicJoystickRChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), x, y);
	}
}

void UWiiRemoteComponent::OnClassicTriggersChangedFunction(int32 wiiRemoteId, float left, float right)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnClassicTriggersChanged.Broadcast(InternalControllerForId(wiiRemoteId), left, right);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnClassicTriggersChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), left, right);
	}
}

void UWiiRemoteComponent::OnBalanceConnectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnBalanceConnected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnBalanceConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnBalanceWeightChangedFunction(int32 wiiRemoteId, FWiiRemoteBalanceBoard balanceBoard)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnBalanceWeightChanged.Broadcast(InternalControllerForId(wiiRemoteId), balanceBoard);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnBalanceWeightChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), balanceBoard);
	}
}

void UWiiRemoteComponent::OnMotionPlusDetectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnMotionPlusDetected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnMotionPlusDetected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnMotionPlusEnabledFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnMotionPlusEnabled.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnMotionPlusEnabled(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnMotionPlusSpeedChangedFunction(int32 wiiRemoteId, FRotator speed)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnMotionPlusSpeedChanged.Broadcast(InternalControllerForId(wiiRemoteId), speed);
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnMotionPlusSpeedChanged(InterfaceDelegate, InternalControllerForId(wiiRemoteId), speed);
	}
}

void UWiiRemoteComponent::OnMotionPlusExtensionConnectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnMotionPlusExtensionConnected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnMotionPlusExtensionConnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnMotionPlusExtensionDisconnectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnMotionPlusExtensionDisconnected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnMotionPlusExtensionDisconnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}

void UWiiRemoteComponent::OnExtensionDisconnectedFunction(int32 wiiRemoteId)
{
	if (IsWiiRemoteValidId(wiiRemoteId))
	{
		OnExtensionDisconnected.Broadcast(InternalControllerForId(wiiRemoteId));
		if (GetOwner()->GetClass()->ImplementsInterface(UWiiRemoteInterface::StaticClass()))
			IWiiRemoteInterface::Execute_OnExtensionDisconnected(InterfaceDelegate, InternalControllerForId(wiiRemoteId));
	}
}
