// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class IWiiRemotePlugin : public IModuleInterface
{

public:

	static inline IWiiRemotePlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IWiiRemotePlugin >( "WiiRemotePlugin" );
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "WiiRemotePlugin" );
	}
};

