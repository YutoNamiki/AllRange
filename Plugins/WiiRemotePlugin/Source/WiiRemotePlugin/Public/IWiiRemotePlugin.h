// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class IWiiRemoteDelegate;
struct FWiiRemoteDeviceData;
struct FWiiRemoteSample;
enum class ReportType : uint8;
enum class WiiRemoteSpeakerFrequency : uint8;
enum class WiiRemoteLED : uint8;

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
	virtual void SetReportType(int32 playerIndex, ReportType reportType) { }
	virtual void SetLED(int32 playerIndex, WiiRemoteLED ledBits) { }
	virtual void SetRumble(int32 playerIndex, bool on) { }
	virtual void SetRumbleForAsync(int32 playerIndex, int32 milliseconds) { }
	virtual void EnableSpeaker(int32 playerIndex, bool on) { }
	virtual void PlaySquareWave(int32 playerIndex, WiiRemoteSpeakerFrequency frequency, int32 volume) { }
	virtual void PlaySample(int32 playerIndex, FWiiRemoteSample* sample, WiiRemoteSpeakerFrequency frequency, int32 volume) { }
	virtual FWiiRemoteDeviceData* LatestData(int32 wiiRemoteId) { return nullptr; }
	virtual void MaxWiiRemoteId(int32& wiiRemoteId) { }
	virtual bool IsValidDeviceId(int32 deviceId) = 0;

};

