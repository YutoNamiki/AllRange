// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyoDelegate.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(MyoPluginLog, Log, All);

class UMyoController;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectSignature, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisconnectSignature, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPairSignature, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnPairSignature, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FArmMovedSignature, UMyoController*, Myo, FVector, ArmAcceleration, FRotator, ArmOrientation, FVector, ArmGyro, MyoPose, Pose);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FQuaternionDataSignature, UMyoController*, Myo, FQuat, Quaternion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOrientationDataSignature, UMyoController*, Myo, FRotator, Rotation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAccelerometerDataSignature, UMyoController*, Myo, FVector, Acceleration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGyroscopeDataSignature, UMyoController*, Myo, FVector, Gyro);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPoseSignature, UMyoController*, Myo, MyoPose, Pose);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FArmSyncSignature, UMyoController*, Myo, MyoArm, Arm, MyoArmDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArmUnsyncSignature, UMyoController*, Myo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEmgDataSignature, UMyoController*, Myo, FMyoEmgData, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisabledSignature);

UINTERFACE(MinimalAPI)
class UMyoDelegate : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IMyoDelegate
{
	GENERATED_IINTERFACE_BODY()

public:
	UObject* ValidSelfPointer;
	UObject* InterfaceDelegate;
	TArray<UMyoController*> LatestFrame;

	virtual UMyoController* MyoPrimaryMyo();
	virtual UMyoController* MyoLeftMyo();
	virtual UMyoController* MyoRightMyo();
	virtual void OnConnectFunction(int32 myoId);
	virtual void OnDisconnectFunction(int32 myoId);
	virtual void OnPairFunction(int32 myoId);
	virtual void OnUnpairFunction(int32 myoId);
	virtual void OnArmMovedFunction(int32 myoId, FVector armAcceleration, FRotator armOrientation, FVector armGyro, MyoPose pose);
	virtual void OnOrientationDataFunction(int32 myoId, FQuat quat);
	virtual void OnOrientationDataFunction(int32 myoId, FRotator rot);
	virtual void OnAccelerometerDataFunction(int32 myoId, FVector accel);
	virtual void OnGyroscopeDataFunction(int32 myoId, FVector gyro);
	virtual void OnPoseFunction(int32 myoId, MyoPose pose);
	virtual void OnArmSyncFunction(int32 myoId, MyoArm arm, MyoArmDirection direction);
	virtual void OnArmUnsyncFunction(int32 myoId);
	virtual void OnEmgDataFunction(int32 myoId, FMyoEmgData data);
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

	void SetInterfaceDelegate(UObject* newDelegate);
	bool IsValidDelegate();
	UMyoController* InternalAddController(int32 newId);
	UMyoController* InternalControllerForId(int32 myoId);

};
