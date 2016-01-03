#pragma once

#include "Components/ActorComponent.h"
#include "HAL/ThreadingBase.h"
#include "PathFindingComponent.generated.h"

struct FWaypoint;

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
	

	static bool LineTracingStartAndEnd(FCriticalSection* mutex, UWorld* world, FPathFindingData& orderData);
	static int32 GetNearestNode(TArray<FWaypoint>* waypointList, FVector location);
	static bool LoadFromDataMap(FCriticalSection* mutex, TMap<FString, TArray<FVector>>& data, int32 startId, int32 endId, FPathFindingData& orderData);
	static bool FindPathByAStar(FCriticalSection* mutex, TArray<FWaypoint>* waypointList, PathFindingInformation& pathInfo, FPathFindingData& orderData);
	static int32 GetMinCostNodeID(TArray<FWaypoint>* waypointList, TArray<int32>& idArray);
	static void ConvertResultPath(FCriticalSection* mutex, TArray<FWaypoint>* waypointList, PathFindingInformation& pathInfo, FPathFindingData& orderData);
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
