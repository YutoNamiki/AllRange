// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyoDelegate.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(MyoPluginLog, Log, All);

UENUM(BlueprintType)
enum class MyoArm : uint8
{
	Right,
	Left,
	Unknown
};

UENUM(BlueprintType)
enum class MyoArmDirection : uint8
{
	Wrist,
	Elbow,
	Unknown
};

UENUM(BlueprintType)
enum class MyoPose : uint8
{
	Rest,
	Fist,
	WaveIn,
	WaveOut,
	FingerSpread,
	DoubleTap,
	Unknown
};

UENUM(BlueprintType)
enum class MyoVibrationType : uint8
{
	Short,
	Medium,
	Long
};

UENUM(BlueprintType)
enum class MyoLockingPolicy : uint8
{
	None,
	Standard
};

UENUM(BlueprintType)
enum class MyoUnlockType : uint8
{
	Timed,
	Hold
};

UENUM(BlueprintType)
enum class MyoStreamEmgType : uint8
{
	Disabled,
	Enabled
};

USTRUCT(BlueprintType)
struct FMyoEmgData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emg Data Struct")
	TArray<int32> Streams;
};

USTRUCT(BlueprintType)
struct FMyoDeviceData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	MyoPose Pose;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FVector Acceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FQuat Quaternion;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FRotator Orientation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FVector Gyro;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	MyoArm Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	MyoArmDirection XDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FVector ArmAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FRotator ArmOrientation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FVector ArmGyro;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FRotator ArmSpaceCorrection;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	FVector BodySpaceNullAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Myo Data")
	bool IsLocked;
};

UINTERFACE(MinimalAPI)
class UMyoDelegate : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IMyoDelegate
{
	GENERATED_IINTERFACE_BODY()

	virtual void OnConnect(int32 myoId);
	virtual void OnDisconnect(int32 myoId);
	virtual void OnPair(int32 myoId);
	virtual void OnUnpair(int32 myoId);
	virtual void OnArmMoved(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose);
	virtual void OnOrientationData(int32 myoId, FQuat quat);
	virtual void OnOrientationData(int32 myoId, FRotator rot);
	virtual void OnAccelerometerData(int32 myoId, FVector accel);
	virtual void OnGyroscopeData(int32 myoId, FVector gyro);
	virtual void OnPose(int32 myoId, MyoPose pose);
	virtual void OnArmSync(int32 myoId, MyoArm arm, MyoArmDirection direction);
	virtual void OnArmUnsync(int32 myoId);
	virtual void OnEmgData(int32 myoId, FMyoEmgData data);
	virtual void MyoDisabled();
	virtual void MyoVibrateDevice(int32 myoId, MyoVibrationType type);
	virtual void MyoSetLockingPolicy(MyoLockingPolicy policy);
	virtual void MyoSetStreamEmg(int deviceId, MyoStreamEmgType type);
	virtual void MyoUnlockMyo(int deviceId, MyoUnlockType type);
	virtual void MyoLockMyo(int deviceId);
	virtual bool MyoIsHubEnabled();
	virtual FMyoDeviceData* MyoLatestData(int32 myoId);
	virtual void MyoWhichArm(int32 myoId, MyoArm& Arm);
	virtual void MyoLeftMyoId(bool& available, int32& myoId);
	virtual void MyoRightMyoId(bool& available, int32& myoId);
	virtual void MyoPrimaryMyoId(bool& available, int32& myoId);
	virtual int32 MyoMaxId();
	virtual bool MyoIsValidId(int32 myoId);
	virtual void MyoConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted);
	virtual void MyoCalibrateArmOrientation(int32 myoId, FRotator direction);
	virtual void MyoStartup();
	virtual void MyoShutdown();
	virtual void MyoTick(float DeltaTime);
};
