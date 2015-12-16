// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "WiiRemoteDelegateBlueprint.h"
#include "WiiRemoteComponent.generated.h"
 
UCLASS(Blueprintable, ClassGroup = "Input Controller", meta = (BlueprintSpawnableComponent))
class WIIREMOTEPLUGIN_API UWiiRemoteComponent : public UActorComponent, public IWiiRemoteDelegateBlueprint
{
	GENERATED_BODY()

public:
	UWiiRemoteComponent(const FObjectInitializer& init);
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
	
};
