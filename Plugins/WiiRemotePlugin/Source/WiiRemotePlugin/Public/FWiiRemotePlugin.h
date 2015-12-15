#pragma once

#include "IWiiRemotePlugin.h"

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

class FWiiRemotePlugin : public IWiiRemotePlugin
{
public:


	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
