// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "WiiRemotePluginPrivatePCH.h"

#include <AllowWindowsPlatformTypes.h>
#include "wiimote.h"
#include <HideWindowsPlatformTypes.h>

struct EWiiRemoteKeys
{
	static const FKey A;
	static const FKey B;
	static const FKey Plus;
	static const FKey Home;
	static const FKey Minus;
	static const FKey One;
	static const FKey Two;
	static const FKey Up;
	static const FKey Down;
	static const FKey Left;
	static const FKey Right;
	static const FKey AccelerationX;
	static const FKey AccelerationY;
	static const FKey AccelerationZ;
	static const FKey OrientationPitch;
	static const FKey OrientationYaw;
	static const FKey OrientationRoll;
	static const FKey GyroX;
	static const FKey GyroY;
	static const FKey GyroZ;
};

const FKey EWiiRemoteKeys::A("WiiRemote A");
const FKey EWiiRemoteKeys::B("WiiRemote B");
const FKey EWiiRemoteKeys::Plus("WiiRemote Plus");
const FKey EWiiRemoteKeys::Home("WiiRemote Home");
const FKey EWiiRemoteKeys::Minus("WiiRemote Minus");
const FKey EWiiRemoteKeys::One("WiiRemote One");
const FKey EWiiRemoteKeys::Two("WiiRemote Two");
const FKey EWiiRemoteKeys::Up("WiiRemote Up");
const FKey EWiiRemoteKeys::Down("WiiRemote Down");
const FKey EWiiRemoteKeys::Left("WiiRemote Left");
const FKey EWiiRemoteKeys::Right("WiiRemote Right");
const FKey EWiiRemoteKeys::AccelerationX("WiiRemote AccelerationX");
const FKey EWiiRemoteKeys::AccelerationY("WiiRemote AccelerationY");
const FKey EWiiRemoteKeys::AccelerationZ("WiiRemote AccelerationZ");
const FKey EWiiRemoteKeys::OrientationPitch("WiiRemote OrientationPitch");
const FKey EWiiRemoteKeys::OrientationRoll("WiiRemote OrientationRoll");
const FKey EWiiRemoteKeys::GyroX("WiiRemote GyroX");
const FKey EWiiRemoteKeys::GyroY("WiiRemote GyroY");
const FKey EWiiRemoteKeys::GyroZ("WiiRemote GyroZ");

class FWiiRemotePlugin : public IWiiRemotePlugin
{
public:


	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

class WiiRemoteManager
{
	
};

IMPLEMENT_MODULE( FWiiRemotePlugin, WiiRemotePlugin )

void FWiiRemotePlugin::StartupModule()
{
	
}

void FWiiRemotePlugin::ShutdownModule()
{
	
}
