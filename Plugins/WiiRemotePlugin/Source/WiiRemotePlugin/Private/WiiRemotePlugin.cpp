// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "WiiRemotePluginPrivatePCH.h"

#include "WiiRemotePluginObject.h"



class FWiiRemotePlugin : public IWiiRemotePlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FWiiRemotePlugin, WiiRemotePlugin )



void FWiiRemotePlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FWiiRemotePlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



