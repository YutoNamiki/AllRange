// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class IMyoDelegate;
struct FMyoDeviceData;
enum class MyoArm : uint8;
enum class MyoLockingPolicy : uint8;
enum class MyoUnlockType : uint8;
enum class MyoStreamEmgType : uint8;
enum class MyoVibrationType : uint8;

/**
 * The public interface to this module
 */
class IMyoPlugin : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IMyoPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< IMyoPlugin >( "MyoPlugin" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "MyoPlugin" );
	}

	virtual void SetDelegate(IMyoDelegate* newDelegate) {};
	virtual void RemoveDelegate() {};
	virtual void MyoTick(float DeltaTime) {};
	virtual void VibrateDevice(int32 deviceId, MyoVibrationType vibrationType) {};
	virtual FMyoDeviceData* LatestData(int32 deviceId) { return nullptr; };
	virtual void WhichArm(int32 deviceId, MyoArm& arm) {};
	virtual void LeftMyoId(bool& available, int32& deviceId) {};
	virtual void RightMyoId(bool& available, int32& deviceId) {};
	virtual void PrimaryMyoId(bool& available, int32& deviceId) {};
	virtual void MaxMyoId(int32& myoId) {};
	virtual bool IsHubEnabled() = 0;
	virtual bool IsValidDeviceId(int32 deviceId) = 0;
	virtual void CalibrateOrientation(int32 deviceId, FRotator direction) {};
	virtual void SetLockingPolicy(MyoLockingPolicy policy) {};
	virtual void UnlockMyo(int32 deviceId, MyoUnlockType type) {};
	virtual void LockMyo(int32 deviceId) {};
	virtual void SetStreamEmg(int32 deviceId, MyoStreamEmgType type) {};
};

