#pragma once

#include "IWiiRemotePlugin.h"

class UWiiRemoteManager;

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
	static const FKey OrientationRoll;
	static const FKey Dot1_X;
	static const FKey Dot1_Y;
	static const FKey Dot2_X;
	static const FKey Dot2_Y;
	static const FKey Dot3_X;
	static const FKey Dot3_Y;
	static const FKey Dot4_X;
	static const FKey Dot4_Y;
};

struct EWiiRemoteNunchukKeys
{
	static const FKey C;
	static const FKey Z;
	static const FKey AccelerationX;
	static const FKey AccelerationY;
	static const FKey AccelerationZ;
	static const FKey OrientationPitch;
	static const FKey OrientationRoll;
	static const FKey JoystickX;
	static const FKey JoystickY;
};

struct EWiiRemoteClassicControllerKeys
{
	static const FKey A;
	static const FKey B;
	static const FKey X;
	static const FKey Y;
	static const FKey Plus;
	static const FKey Home;
	static const FKey Minus;
	static const FKey Up;
	static const FKey Down;
	static const FKey Right;
	static const FKey Left;
	static const FKey ZL;
	static const FKey ZR;
	static const FKey TriggerL;
	static const FKey TrigerrR;
	static const FKey LeftJoystickX;
	static const FKey LeftJoystickY;
	static const FKey RightJoystickX;
	static const FKey RightJoystickY;
	static const FKey LeftTrigger;
	static const FKey RightTrigger;
};

struct EWiiRemoteBalanceBoardKeys
{
	static const FKey AtRestKilograms_TopL;
	static const FKey AtRestKilograms_TopR;
	static const FKey AtRestKilograms_BottomL;
	static const FKey AtRestKilograms_BottomR;
	static const FKey AtRestKilograms_Total;
	static const FKey Kilograms_TopL;
	static const FKey Kilograms_TopR;
	static const FKey Kilograms_BottomL;
	static const FKey Kilograms_BottomR;
	static const FKey Kilograms_Total;
	static const FKey Pounds_TopL;
	static const FKey Pounds_TopR;
	static const FKey Pounds_BottomL;
	static const FKey Pounds_BottomR;
	static const FKey Pounds_Total;
};

struct EWiiRemoteMotionPlusKeys
{
	static const FKey SpeedOrientationPitch;
	static const FKey SpeedOrientationYaw;
	static const FKey SpeedOrientationRoll;
};

class FWiiRemotePlugin : public IWiiRemotePlugin
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual void SetDelegate(IWiiRemoteDelegate* newDelegate) override;
	virtual void RemoveDelgate() override;
	virtual void Tick(float deltaTime) override;
	virtual void SetLED(int32 playerIndex, WiiRemoteLED ledBits) override;
	virtual void SetRumble(int32 playerIndex, bool on) override;
	virtual void SetRumbleForAsync(int32 playerIndex, int32 milliseconds) override;
	virtual void EnableSpeaker(int32 playerIndex, bool on) override;
	virtual void PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequency, int32 volume) override;
	virtual void PlaySample(int32 playerIndex, FWiiRemoteSample* sample, WiiRemoteSpeakerFrequency frequency, int32 volume) override;
	virtual FWiiRemoteDeviceData* LatestData(int32 wiiRemoteId) override;
	virtual void MaxWiiRemoteId(int32& wiiRemoteId) override;
	virtual bool IsValidDeviceId(int32 deviceId) override;

private:
	UWiiRemoteManager* wiiRemoteManager;
};
