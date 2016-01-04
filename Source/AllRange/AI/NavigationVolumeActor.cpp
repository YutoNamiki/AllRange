#include "AllRange.h"
#include "NavigationVolumeActor.h"
#include "DrawDebugHelpers.h"
#include "PathFindingComponent.h"

ANavigationVolumeActor::ANavigationVolumeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxVolume = CreateDefaultSubobject<UBoxComponent>(FName("Volume"));
	RootComponent = BoxVolume;
	PathFinder = CreateDefaultSubobject<UPathFindingComponent>(FName("PathFindingComponent"));
	PathFinder->WaypointList = &WaypointList;
}

void ANavigationVolumeActor::BeginPlay()
{
	Super::BeginPlay();

	if (IsDebugDrawVolumes)
	{
		for (auto i = 0; i < WaypointCount; i++)
		{
			auto location = (WaypointList[i].MaximumLocation + WaypointList[i].MinimumLocation) * 0.5f;
			auto extent = (WaypointList[i].MaximumLocation - WaypointList[i].MinimumLocation) * 0.5f;
			DrawDebugBox(GetWorld(), location, extent, DrawVolumeColor, true);
		}
	}
	if (IsDebugDrawPaths)
	{
		for (auto i = 0; i < WaypointPathCount; i++)
		{
			auto waypoint1 = WaypointList[WaypointPathList[i].ID1];
			auto waypoint2 = WaypointList[WaypointPathList[i].ID2];
			auto location1 = (waypoint1.MaximumLocation + waypoint1.MinimumLocation) * 0.5f;
			auto location2 = (waypoint2.MaximumLocation + waypoint2.MinimumLocation) * 0.5f;
			DrawDebugLine(GetWorld(), location1, location2, DrawPathColor, true, -1.0f, (uint8)'\000', DrawPathThickness);
		}
	}
}

void ANavigationVolumeActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (Bake)
	{
		Bake = false;
		Initialize();
	}
}

void ANavigationVolumeActor::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
	Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
	if (Bake)
	{
		Bake = false;
		Initialize();
	}
}

void ANavigationVolumeActor::AddPathFindingQue(FPathFindingData addData)
{
	PathFinder->OrderQue.Add(addData);
}

EPathFindingResult ANavigationVolumeActor::GetPathFindingResultRoute(APawn* orderPawn, TArray<FVector>& resultRoute)
{
	if (PathFinder->OrderQue.Num() == 0)
		return EPathFindingResult::Failed;
	const auto num = PathFinder->OrderQue.Num();
	for (auto index = 0; index < num; index++)
	{
		if (PathFinder->OrderQue[index].OrderPawn == orderPawn)
		{
			const auto result = PathFinder->OrderQue[index].Result;
			switch (result)
			{
			case EPathFindingResult::Failed:
				PathFinder->OrderQue.RemoveAt(index);
				return result;
			case EPathFindingResult::Thinking:
				return result;
			case EPathFindingResult::Success:
				resultRoute.Empty();
				resultRoute = PathFinder->OrderQue[index].ResultRoute;
				PathFinder->OrderQue.RemoveAt(index);
				return result;
			}
		}
	}
	return EPathFindingResult::Failed;
}

void ANavigationVolumeActor::Initialize()
{
	WaypointList.Empty();
	WaypointPathList.Empty();
	DestroyChildrenComponents(BoxVolume);
	TArray<TArray<FWaypoint>> createWaypointData;
	TArray<FWaypointPath> createPathData;
	createWaypointData.Init(TArray<FWaypoint>(), Recursion + 1);
	DivideVolume(BoxVolume, DivideX, DivideY, DivideZ, createWaypointData[0]);
	for (auto recursionIndex = 0; recursionIndex <= Recursion; recursionIndex++)
		CreateOctree(&mutex, GetWorld(), Recursion, recursionIndex, createWaypointData);
	SetWaypointList(WaypointList, createWaypointData, Recursion);
	CreatePath(&mutex, WaypointList, createPathData);
	CheckOverlapPaths(&mutex, createPathData);
	SetWaypointPathList(WaypointPathList, createPathData);
	PathFinder->WaypointList = &WaypointList;
	WaypointCount = WaypointList.Num();
	WaypointPathCount = WaypointPathList.Num();
	if (!IsDebugDrawPaths)
		WaypointPathList.Empty();
}

void ANavigationVolumeActor::DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ, TArray<FWaypoint>& createData)
{
	const auto maxPosition = volume->GetComponentLocation() + volume->GetUnscaledBoxExtent() * 1.0f;
	const auto minPosition = volume->GetComponentLocation() + volume->GetUnscaledBoxExtent() * -1.0f;
	const auto divideScale = FVector(static_cast<float>(divX), static_cast<float>(divY), static_cast<float>(divZ));
	const auto deltaVector = (maxPosition - minPosition) / divideScale;

	for (auto xCount = 0; xCount < divX; xCount++)
	{
		const auto x = minPosition.X + deltaVector.X * xCount;
		for (auto yCount = 0; yCount < divY; yCount++)
		{
			const auto y = minPosition.Y + deltaVector.Y * yCount;
			for (auto zCount = 0; zCount < divZ; zCount++)
			{
				const auto z = minPosition.Z + deltaVector.Z * zCount;
				const auto min = FVector(x, y, z);
				const auto max = min + deltaVector;
				createData.Add(CreateWaypoint(min, max));
			}
		}
	}
}

void ANavigationVolumeActor::DestroyChildrenComponents(USceneComponent * component)
{
	while (component->GetNumChildrenComponents() > 0)
	{
		const auto childComponent = component->GetChildComponent(0);
		if (childComponent == nullptr)
			continue;
		DestroyChildrenComponents(childComponent);
		if (childComponent->IsRegistered())
			childComponent->UnregisterComponent();
		childComponent->DestroyComponent(false);
	}
}

FWaypoint ANavigationVolumeActor::CreateWaypoint(FVector minLocation, FVector maxLocation)
{
	return FWaypoint{ -1, EWaypointState::None, 0.0f, TArray<int32>(), -1, minLocation, maxLocation, true };
}

void ANavigationVolumeActor::CreateOctree(FCriticalSection* mutex, UWorld* world, int32 recursion, int32 recursionIndex, TArray<TArray<FWaypoint>>& createData)
{
	static auto number = 0;
	const auto& num = createData[recursionIndex].Num();
	TArray<FRunnableThread*> threads;
	TArray<CreateOctreeWorker*> workers;
	for (auto i = 0; i < num; i++)
	{
		const auto threadName = FString("CreateOctreeWorkerThread_") + FString::FromInt(number++);
		workers.Add(new CreateOctreeWorker(mutex, world, createData, recursion, recursionIndex, i));
		threads.Add(FRunnableThread::Create(workers[i], *threadName));
	}
	for (auto i = 0; i < num; i++)
	{
		threads[i]->WaitForCompletion();
		threads[i]->Kill();
		delete workers[i];
		delete threads[i];
	}
}

void ANavigationVolumeActor::SetWaypointList(TArray<FWaypoint>& waypoints, TArray<TArray<FWaypoint>>& waypointData, int32 recursion)
{
	for (auto recursionIndex = 0; recursionIndex <= recursion; recursionIndex++)
	{
		const auto num = waypointData[recursionIndex].Num();
		for (auto index = 0; index < num; index++)
		{
			auto waypoint = waypointData[recursionIndex][index];
			if (waypoint.Enabled)
			{
				waypoints.Add(waypoint);
				waypoints.Last().ID = waypoints.Num() - 1;
			}
		}
	}
}

void ANavigationVolumeActor::CreatePath(FCriticalSection* mutex, TArray<FWaypoint>& waypoints, TArray<FWaypointPath>& waypointPathData)
{
	static auto number = 0;
	const auto& num = waypoints.Num();
	TArray<FRunnableThread*> threads;
	TArray<CreatePathWorker*> workers;
	for (auto i = 0; i < num; i++)
	{
		const auto threadName = FString("CreatePathWorkerThread_") + FString::FromInt(number++);
		workers.Add(new CreatePathWorker(mutex, waypoints[i], &waypointPathData, &waypoints));
		threads.Add(FRunnableThread::Create(workers[i], *threadName));
	}
	for (auto i = 0; i < num; i++)
	{
		threads[i]->WaitForCompletion();
		threads[i]->Kill();
		delete workers[i];
		delete threads[i];
	}
}

void ANavigationVolumeActor::CheckOverlapPaths(FCriticalSection* mutex, TArray<FWaypointPath>& waypointPathData)
{
	static auto number = 0;
	const auto& num = waypointPathData.Num();
	TArray<FRunnableThread*> threads;
	TArray<CheckOverlapPathWorker*> workers;
	for (auto i = 0; i < num; i++)
	{
		const auto threadName = FString("CheckOverlapPathsThread_") + FString::FromInt(number++);
		workers.Add(new CheckOverlapPathWorker(mutex, waypointPathData[i], &waypointPathData));
		threads.Add(FRunnableThread::Create(workers[i], *threadName));
	}
	for (auto i = 0; i < num; i++)
	{
		threads[i]->WaitForCompletion();
		threads[i]->Kill();
		delete workers[i];
		delete threads[i];
	}
}

void ANavigationVolumeActor::SetWaypointPathList(TArray<FWaypointPath>& waypointPaths, TArray<FWaypointPath>& waypointPathData)
{
	const auto& num = waypointPathData.Num();
	for (auto i = 0; i < num; i++)
	{
		auto path = waypointPathData[i];
		if (path.Enabled)
			waypointPaths.Add(path);
	}
}

CreateOctreeWorker::CreateOctreeWorker(FCriticalSection* mutex, UWorld* world, TArray<TArray<FWaypoint>>& data, int32 recursion, int32 recursionIndex, int32 index)
{
	this->mutex = mutex;
	this->world = world;
	waypointData = &data;
	this->recursion = recursion;
	this->recursionIndex = recursionIndex;
	this->index = index;
}

uint32 CreateOctreeWorker::Run()
{
	auto waypoint = (*waypointData)[recursionIndex][index];
	auto min = waypoint.MinimumLocation;
	auto max = waypoint.MaximumLocation;
	auto position = (max + min) * 0.5f;
	auto extent = (max - min) * 0.5f;
	if (!world->OverlapAnyTestByObjectType(position, FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic), FCollisionShape::MakeBox(extent)))
		return 0;
	FVector minLocations[8];
	FVector maxLocations[8];
	minLocations[0] = FVector(min.X, min.Y, min.Z);
	minLocations[1] = FVector(position.X, min.Y, min.Z);
	minLocations[2] = FVector(min.X, min.Y, position.Z);
	minLocations[3] = FVector(position.X, min.Y, position.Z);
	minLocations[4] = FVector(min.X, position.Y, min.Z);
	minLocations[5] = FVector(position.X, position.Y, min.Z);
	minLocations[6] = FVector(min.X, position.Y, position.Z);
	minLocations[7] = FVector(position.X, position.Y, position.Z);
	maxLocations[0] = FVector(position.X, position.Y, position.Z);
	maxLocations[1] = FVector(max.X, position.Y, position.Z);
	maxLocations[2] = FVector(position.X, position.Y, max.Z);
	maxLocations[3] = FVector(max.X, position.Y, max.Z);
	maxLocations[4] = FVector(position.X, max.Y, position.Z);
	maxLocations[5] = FVector(max.X, max.Y, position.Z);
	maxLocations[6] = FVector(position.X, max.Y, max.Z);
	maxLocations[7] = FVector(max.X, max.Y, max.Z);
	{
		FScopeLock lock(mutex);
		(*waypointData)[recursionIndex][index].Enabled = false;
		if (recursionIndex >= recursion)
			return 0;
		for (auto i = 0; i < 8; i++)
			(*waypointData)[recursionIndex + 1].Add(FWaypoint{ -1, EWaypointState::None, 0.0f, TArray<int32>(), -1, minLocations[i], maxLocations[i], true });
	}
	return 0;
}

CreatePathWorker::CreatePathWorker(FCriticalSection* mutex, FWaypoint& waypoint, TArray<FWaypointPath>* waypointPathList, TArray<FWaypoint>* waypointList)
{
	this->mutex = mutex;
	this->waypoint = &waypoint;
	this->waypointList = waypointList;
	this->waypointPathList = waypointPathList;
}

uint32 CreatePathWorker::Run()
{
	const auto num = waypointList->Num();
	const auto oneVector = FVector(1.0f, 1.0f, 1.0f);
	auto id = waypoint->ID;
	for (auto i = 0; i < num; i++)
	{
		const auto& other = (*waypointList)[i];
		if (IsIntersect(waypoint->MinimumLocation - oneVector, waypoint->MaximumLocation + oneVector,
			other.MinimumLocation - oneVector, other.MaximumLocation + oneVector))
		{
			auto otherID = other.ID;
			FScopeLock lock(mutex);
			waypoint->NeighborWaypointIDs.Add(otherID);
			waypointPathList->Add(FWaypointPath{ id, otherID, true });
		}
	}
	return 0;
}

bool CreatePathWorker::IsIntersect(FVector min1, FVector max1, FVector min2, FVector max2)
{
	if (min1.X > max2.X || min2.X > max1.X)
		return false;
	if (min1.Y > max2.Y || min2.Y > max1.Y)
		return false;
	if (min1.Z > max2.Z || min2.Z > max1.Z)
		return false;
	return true;
}

CheckOverlapPathWorker::CheckOverlapPathWorker(FCriticalSection* mutex, FWaypointPath& path, TArray<FWaypointPath>* waypointPathList)
{
	this->mutex = mutex;
	this->path = &path;
	this->waypointPathList = waypointPathList;
}

uint32 CheckOverlapPathWorker::Run()
{
	auto num = waypointPathList->Num();
	for (auto i = 0; i < num; i++)
	{
		auto& other = (*waypointPathList)[i];
		if (path->ID1 == other.ID1 && path->ID2 == other.ID2)
			continue;
		if (path->ID1 == other.ID2 && path->ID2 == other.ID1)
		{
			FScopeLock lock(mutex);
			if (path->Enabled)
				other.Enabled = false;
		}
	}
	return 0;
}
