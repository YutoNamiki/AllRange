// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteComponent.h"

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

void UWiiRemoteComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);
	IWiiRemoteDelegateBlueprint::Tick(deltaTime);
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
