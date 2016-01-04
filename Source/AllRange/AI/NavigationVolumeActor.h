#pragma once

#include "GameFramework/Actor.h"
#include "HAL/ThreadingBase.h"
#include "NavigationVolumeActor.generated.h"

class UPathFindingComponent;

UENUM(BlueprintType)
enum class EWaypointState : uint8
{
	None	UMETA(DisplayName = "None"),
	Open	UMETA(DisplayName = "Open"),
	Closed	UMETA(DisplayName = "Closed")
};

UENUM(BlueprintType)
enum class EPathFindingResult : uint8
{
	Failed		UMETA(DisplayName = "Failed"),
	Thinking	UMETA(DisplayName = "Thinking"),
	Success		UMETA(DisplayName = "Success")
};

USTRUCT(BlueprintType)
struct FPathFindingData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* OrderPawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPathFindingResult Result;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector> ResultRoute;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	bool Enabled;

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
	bool Enabled;

};

class CreateOctreeWorker : public FRunnable
{
public:
	CreateOctreeWorker(FCriticalSection* mutex, UWorld* world, TArray<TArray<FWaypoint>>& data, int32 recursion, int32 recursionIndex, int32 index);
	virtual uint32 Run() override;

private:
	FCriticalSection* mutex;
	UWorld* world;
	TArray<TArray<FWaypoint>>* waypointData;
	int32 recursionIndex;
	int32 recursion;
	int32 index;

};

class CreatePathWorker : public FRunnable
{
public:
	CreatePathWorker(FCriticalSection* mutex, FWaypoint& waypoint, TArray<FWaypointPath>* waypointPathList, TArray<FWaypoint>* waypointList);
	virtual uint32 Run() override;

private:
	FCriticalSection* mutex;
	TArray<FWaypointPath>* waypointPathList;
	TArray<FWaypoint>* waypointList;
	FWaypoint* waypoint;

	static bool IsIntersect(FVector min1, FVector max1, FVector min2, FVector max2);

};

class CheckOverlapPathWorker : public FRunnable
{
public:
	CheckOverlapPathWorker(FCriticalSection* mutex, FWaypointPath& path, TArray<FWaypointPath>* waypointPathList);
	virtual uint32 Run() override;

private:
	FCriticalSection* mutex;
	TArray<FWaypointPath>* waypointPathList;
	FWaypointPath* path;
};

UCLASS()
class ALLRANGE_API ANavigationVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NavigationVolume")
	UBoxComponent* BoxVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PathFinding")
	UPathFindingComponent* PathFinder;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<FWaypoint> WaypointList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	TArray<FWaypointPath> WaypointPathList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 WaypointCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Waypoint")
	int32 WaypointPathCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideX = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideY = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 DivideZ = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	int32 Recursion = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpaceDivision")
	bool Bake = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugSettings")
	bool IsDebugDrawVolumes = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugSettings")
	bool IsDebugDrawPaths = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugSettings")
	FColor DrawVolumeColor = FColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugSettings")
	FColor DrawPathColor = FColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DebugSettings")
	float DrawPathThickness = 5.0f;

	ANavigationVolumeActor();
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
#endif

	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	void AddPathFindingQue(FPathFindingData addData);
	UFUNCTION(BlueprintCallable, Category = "PathFinding")
	EPathFindingResult GetPathFindingResultRoute(APawn* orderPawn, TArray<FVector>& resultRoute);

private:
	FCriticalSection mutex;

	void Initialize();

	static void DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ, TArray<FWaypoint>& createData);
	static void DestroyChildrenComponents(USceneComponent* component);
	static FWaypoint CreateWaypoint(FVector minLocation, FVector maxLocation);
	static void CreateOctree(FCriticalSection* mutex, UWorld* world, int32 recursion, int32 recursionIndex, TArray<TArray<FWaypoint>>& createData);
	static void SetWaypointList(TArray<FWaypoint>& waypoints, TArray<TArray<FWaypoint>>& waypointData, int32 recursion);
	static void CreatePath(FCriticalSection* mutex, TArray<FWaypoint>& waypoints, TArray<FWaypointPath>& waypointPathData);
	static void CheckOverlapPaths(FCriticalSection* mutex, TArray<FWaypointPath>& waypointPathData);
	static void SetWaypointPathList(TArray<FWaypointPath>& waypointPaths, TArray<FWaypointPath>& waypointPathData);

};
