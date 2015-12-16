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
