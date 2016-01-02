// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HAL/ThreadingBase.h"
#include "NavigationVolumeActor.generated.h"

UENUM(BlueprintType)
enum class EWaypointState : uint8
{
	None	UMETA(DisplayName = "None"),
	Open	UMETA(DisplayName = "Open"),
	Closed	UMETA(DisplayName = "Closed")
};

USTRUCT(BlueprintType)
struct FWaypoint
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 ID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	EWaypointState State;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	float Cost;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<int32> NeighborWaypointIDs;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 ParentWaypointID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	FVector MinimumLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	FVector MaximumLocation;

};

USTRUCT(BlueprintType)
struct FWaypointPath
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	int32 ID1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	int32 ID2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	FColor DrawColor;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationPath")
	float Thickness = 1.0f;

};

class CreateOctreeWorker : FRunnable
{
public:
	CreateOctreeWorker(FCriticalSection& mutex, TArray<TArray<FWaypoint>>& data, int32 recursionIndex);
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FThreadSafeCounter stopTaskCounter;
	FCriticalSection* mutex;
	TArray<TArray<FWaypoint>>* waypointData;
	int32 recursionIndex;
};

UCLASS()
class ALLRANGE_API ANavigationVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NavigationVolume")
	UBoxComponent* BoxVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<FWaypoint> WaypointList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<FWaypointPath> WaypointPathList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 WaypointCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 WaypointPathCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideZ = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 Recursion = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	bool Bake = true;

	ANavigationVolumeActor();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

private:

	void Initialize();

	static void DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ, TArray<FWaypoint>& createData);
	static void DestroyChildrenComponents(USceneComponent* component);
	static FWaypoint CreateWaypoint(FVector minLocation, FVector maxLocation, int32 ID);
	static void CreateOctree(int32 recursionIndex, TArray<FWaypoint>& createData);

};
