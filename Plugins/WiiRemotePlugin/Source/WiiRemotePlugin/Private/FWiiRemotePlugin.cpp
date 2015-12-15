#include "WiiRemotePluginPrivatePCH.h"
#include "FWiiRemotePlugin.h"
#include "WiiRemoteManager.h"

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

IMPLEMENT_MODULE(FWiiRemotePlugin, WiiRemotePlugin)

void FWiiRemotePlugin::StartupModule()
{

}

void FWiiRemotePlugin::ShutdownModule()
{

}

