// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class IWiiRemoteDelegate;
struct FWiiRemoteDeviceData;
enum class WiiRemoteSpeakerFrequency : uint8;


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

	virtual void SetDelegate(IWiiRemoteDelegate* newDelegate) { }
	virtual void RemoveDelgate() { }
	virtual void Tick(float deltaTime) { }
	virtual void SetRumble(bool on) { }
	virtual void SetRumbleForAsync(int32 milliseconds) { }
	virtual void MuteSpeaker(bool on) { }
	virtual void PlaySquareWave(WiiRemoteSpeakerFrequency frequency, int32 volume) { }
	virtual void PlaySample(WiiRemoteSpeakerFrequency frequency, int32 volume) { }
	virtual FWiiRemoteDeviceData* LatestData(int32 wiiRemoteId) { return nullptr; }
	virtual void MaxWiiRemoteId(int32& wiiRemoteId) { }
	virtual bool IsValidDeviceId(int32 deviceId) = 0;

};

