#include "AllRange.h"
#include "PathFindingComponent.h"
#include "NavigationVolumeActor.h"

UPathFindingComponent::UPathFindingComponent()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UPathFindingComponent::BeginPlay()
{
	Super::BeginPlay();
	pathFindingWorker = new PathFindingWorker(&mutex, GetWorld(), WaypointList, OrderQue);
	pathFindingThread = FRunnableThread::Create(pathFindingWorker, TEXT("PathFindingThread"));
}

void UPathFindingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UPathFindingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	pathFindingWorker->Stop();
	pathFindingThread->WaitForCompletion();
	pathFindingThread->Kill();
	delete pathFindingWorker;
	delete pathFindingThread;
}

void UPathFindingComponent::RemoveOrderQue(int32 index)
{
	mutex.Lock();
	OrderQue.RemoveAt(index);
	mutex.Unlock();
}

PathFindingWorker::PathFindingWorker(FCriticalSection* mutex, UWorld* world, TArray<FWaypoint>* waypointList, TArray<FPathFindingData>& currentOrder)
{
	this->mutex = mutex;
	this->world = world;
	this->waypointList = waypointList;
	this->currentOrder = &currentOrder;
}

bool PathFindingWorker::Init()
{
	pathInfo.RouteData.Empty();
	pathInfo.OpenList.Empty();
	pathInfo.CloseList.Empty();
	pathInfo.StartNode = -1;
	pathInfo.EndNode = -1;
	return true;
}

uint32 PathFindingWorker::Run()
{
	while (safeCounter.GetValue() == 0)
	{
		FPathFindingData order;
		order.Result = EPathFindingResult::Failed;
		{
			FScopeLock lock(mutex);
			if (currentOrder == nullptr)
				continue;
			if (currentOrder->GetData() == nullptr)
				continue;
			if (currentOrder->Num() > 0)
			{
				auto orderQue = *currentOrder;
				if (orderQue.Num() > 0)
					order = orderQue[0];
			}
		}
		if (order.Result != EPathFindingResult::Thinking)
			continue;
		if (LineTracingStartAndEnd(world, order))
		{
			FScopeLock lock(mutex);
			(*currentOrder)[0] = order;
			continue;
		}
		pathInfo.StartLocation = order.StartLocation;
		pathInfo.EndLocation = order.TargetLocation;
		pathInfo.StartNode = GetNearestNode(waypointList, pathInfo.StartLocation);
		pathInfo.EndNode = GetNearestNode(waypointList, pathInfo.EndLocation);
		if (LoadFromDataMap(pathInfo.RouteData, pathInfo.StartNode, pathInfo.EndNode, order))
		{
			FScopeLock lock(mutex);
			(*currentOrder)[0] = order;
			continue;
		}
		if (FindPathByAStar(waypointList, pathInfo, order))
		{
			FScopeLock lock(mutex);
			(*currentOrder)[0] = order;
			continue;
		}
		pathInfo.CloseList.Empty();
		pathInfo.OpenList.Empty();
		pathInfo.StartNode = -1;
		pathInfo.EndNode = -1;
		order.Result = EPathFindingResult::Failed;
		FScopeLock lock(mutex);
		(*currentOrder)[0] = order;
	}
	return 0;
}

void PathFindingWorker::Stop()
{
	safeCounter.Increment();
}

bool PathFindingWorker::LineTracingStartAndEnd(UWorld* world, FPathFindingData& orderData)
{
	if (world->LineTraceTestByChannel(orderData.StartLocation, orderData.TargetLocation, ECollisionChannel::ECC_WorldStatic))
		return false;
	orderData.ResultRoute.Empty();
	orderData.ResultRoute.Add(orderData.TargetLocation);
	orderData.Result = EPathFindingResult::Success;
	return true;
}

int32 PathFindingWorker::GetNearestNode(TArray<FWaypoint>* waypointList, FVector location)
{
	const auto num = waypointList->Num();
	const auto firstLocation = ((*waypointList)[0].MaximumLocation + (*waypointList)[0].MinimumLocation) * 0.5f;
	auto distance = FVector::Dist(location, firstLocation);
	auto id = (*waypointList)[0].ID;
	for (auto i = 0; i < num; i++)
	{
		const auto currentNodeLocation = ((*waypointList)[i].MaximumLocation + (*waypointList)[i].MinimumLocation) * 0.5f;
		const auto currentDistance = FVector::Dist(location, currentNodeLocation);
		if (currentDistance <= distance)
		{
			distance = currentDistance;
			id = (*waypointList)[i].ID;
		}
	}
	return id;
}

bool PathFindingWorker::LoadFromDataMap(TMap<FString, TArray<FVector>>& data, int32 startId, int32 endId, FPathFindingData& orderData)
{
	auto keyString = FString::FromInt(startId) + FString("_") + FString::FromInt(endId);
	if (data.Num() == 0)
		return false;
	if (!data.Contains(keyString))
		return false;
	orderData.ResultRoute.Empty();
	orderData.ResultRoute = data[keyString];
	orderData.Result = EPathFindingResult::Success;
	return true;
}

bool PathFindingWorker::FindPathByAStar(TArray<FWaypoint>* waypointList, PathFindingInformation& pathInfo, FPathFindingData& orderData)
{
	const auto startNodeLocation = GetWaypointPosition((*waypointList)[pathInfo.StartNode]);
	const auto endNodeLocation = GetWaypointPosition((*waypointList)[pathInfo.EndNode]);
	pathInfo.OpenList.Add(pathInfo.StartNode);
	while (pathInfo.OpenList.Num() > 0)
	{
		const auto currentNodeID = GetMinCostNodeID(waypointList, pathInfo.OpenList);
		auto& currentNode = (*waypointList)[currentNodeID];
		const auto currentNodeLocation = GetWaypointPosition(currentNode);
		currentNode.Cost = 0.0f;
		currentNode.State = EWaypointState::Closed;
		pathInfo.CloseList.Add(currentNodeID);
		pathInfo.OpenList.Remove(currentNodeID);
		if (currentNode.ID == pathInfo.EndNode)
		{
			ConvertResultPath(waypointList, pathInfo, orderData);
			return true;
		}
		for (auto neighborID : currentNode.NeighborWaypointIDs)
		{
			if (pathInfo.CloseList.Contains(neighborID))
				continue;
			auto& neighborNode = (*waypointList)[neighborID];
			const auto neighborLocation = GetWaypointPosition(neighborNode);
			const auto startToCurrent = FVector::Dist(startNodeLocation, currentNodeLocation);
			const auto endToNeighbor = FVector::Dist(endNodeLocation, neighborLocation);
			const auto neighborToCurrent = FVector::Dist(neighborLocation, currentNodeLocation);
			const auto cost = startToCurrent + endToNeighbor + neighborToCurrent;
			if (cost < neighborNode.Cost || neighborNode.Cost == 0.0f)
			{
				neighborNode.Cost = cost;
				neighborNode.ParentWaypointID = currentNodeID;
			}
			if (!pathInfo.OpenList.Contains(neighborID))
			{
				neighborNode.State = EWaypointState::Open;
				pathInfo.OpenList.Add(neighborID);
			}
		}
	}
	return false;
}

int32 PathFindingWorker::GetMinCostNodeID(TArray<FWaypoint>* waypointList, TArray<int32>& idArray)
{
	const auto num = idArray.Num();
	auto cost = (*waypointList)[idArray[0]].Cost;
	auto id = (*waypointList)[idArray[0]].ID;
	for (auto index = 0; index < num; index++)
	{
		const auto currentCost = (*waypointList)[idArray[index]].Cost;
		if (currentCost < cost)
		{
			cost = currentCost;
			id = (*waypointList)[idArray[index]].ID;
		}
	}
	return id;
}

bool PathFindingWorker::ConvertResultPath(TArray<FWaypoint>* waypointList, PathFindingInformation& pathInfo, FPathFindingData& orderData)
{
	auto result = true;
	const auto endLocation = GetWaypointPosition((*waypointList)[pathInfo.EndNode]);
	orderData.ResultRoute.Empty();
	orderData.ResultRoute.Add(pathInfo.EndLocation);
	orderData.ResultRoute.Add(endLocation);
	auto currentNode = pathInfo.EndNode;
	while (currentNode != pathInfo.StartNode)
	{
		currentNode = (*waypointList)[currentNode].ParentWaypointID;
		if (currentNode != pathInfo.StartNode && currentNode == -1)
		{
			result = false;
			break;
		}
		auto current = (*waypointList)[currentNode];
		auto position = GetWaypointPosition(current);
		orderData.ResultRoute.Add(position);
	}
	for (auto nodeID : pathInfo.OpenList)
	{
		auto& node = (*waypointList)[nodeID];
		node.State = EWaypointState::None;
		node.Cost = 0.0f;
		node.ParentWaypointID = -1;
	}
	for (auto nodeID : pathInfo.CloseList)
	{
		auto& node = (*waypointList)[nodeID];
		node.State = EWaypointState::None;
		node.Cost = 0.0f;
		node.ParentWaypointID = -1;
	}
	if (result)
	{
		auto keyString = FString::FromInt(pathInfo.StartNode) + FString("_") + FString::FromInt(pathInfo.EndNode);
		if (!pathInfo.RouteData.Contains(keyString))
			pathInfo.RouteData.Add(keyString, orderData.ResultRoute); 
		orderData.Result = EPathFindingResult::Success;
	}
	else
	{
		orderData.Result = EPathFindingResult::Failed;
	}
	return result;
}

FVector PathFindingWorker::GetWaypointPosition(FWaypoint waypoint)
{
	return (waypoint.MaximumLocation + waypoint.MinimumLocation) * 0.5f;
}
