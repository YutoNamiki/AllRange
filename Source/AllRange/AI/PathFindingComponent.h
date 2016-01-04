#pragma once

#include "Components/ActorComponent.h"
#include "HAL/ThreadingBase.h"
#include "PathFindingComponent.generated.h"

struct FWaypoint;
struct FPathFindingData;

struct PathFindingInformation
{
	int32 StartNode;
	int32 EndNode;
	FVector StartLocation;
	FVector EndLocation;
	TArray<int32> OpenList;
	TArray<int32> CloseList;
	TMap<FString, TArray<FVector>> RouteData;
};

class PathFindingWorker : public FRunnable
{
public:
	PathFindingWorker(FCriticalSection* mutex, UWorld* world, TArray<FWaypoint>* waypointList, TArray<FPathFindingData>& que);
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

private:
	FThreadSafeCounter safeCounter;
	FCriticalSection* mutex;
	UWorld* world;
	TArray<FWaypoint>* waypointList;
	TArray<FPathFindingData>* que;
	PathFindingInformation pathInfo;

	static bool LineTracingStartAndEnd(UWorld* world, FPathFindingData& orderData);
	static int32 GetNearestNode(TArray<FWaypoint>* waypointList, FVector location);
	static bool LoadFromDataMap(TMap<FString, TArray<FVector>>& data, int32 startId, int32 endId, FPathFindingData& orderData);
	static bool FindPathByAStar(TArray<FWaypoint>* waypointList, PathFindingInformation& pathInfo, FPathFindingData& orderData);
	static int32 GetMinCostNodeID(TArray<FWaypoint>* waypointList, TArray<int32>& idArray);
	static void ConvertResultPath(TArray<FWaypoint>* waypointList, PathFindingInformation& pathInfo, FPathFindingData& orderData);
	static FVector GetWaypointPosition(FWaypoint waypoint);
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALLRANGE_API UPathFindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPathFindingData> OrderQue;

	TArray<FWaypoint>* WaypointList;

	UPathFindingComponent();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FCriticalSection mutex;
	FRunnableThread* pathFindingThread;
	PathFindingWorker* pathFindingWorker;

};
