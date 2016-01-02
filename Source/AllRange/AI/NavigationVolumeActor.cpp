// Fill out your copyright notice in the Description page of Project Settings.

#include "AllRange.h"
#include "NavigationVolumeActor.h"


// Sets default values
ANavigationVolumeActor::ANavigationVolumeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxVolume = CreateDefaultSubobject<UBoxComponent>(FName("Volume"));
	RootComponent = BoxVolume;

}

// Called when the game starts or when spawned
void ANavigationVolumeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavigationVolumeActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ANavigationVolumeActor::Initialize()
{
	WaypointList.Empty();
	WaypointPathList.Empty();
	DestroyChildrenComponents(BoxVolume);
	TArray<TArray<FWaypoint>> createWaypointData;
	createWaypointData.Init(TArray<FWaypoint>(), Recursion + 1);
	DivideVolume(BoxVolume, DivideX, DivideY, DivideZ, createWaypointData[0]);

}

void ANavigationVolumeActor::DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ, TArray<FWaypoint>& createData)
{
	const auto maxPosition = volume->GetUnscaledBoxExtent() * 1.0f;
	const auto minPosition = volume->GetUnscaledBoxExtent() * -1.0f;
	const auto divideScale = FVector(static_cast<float>(divX), static_cast<float>(divY), static_cast<float>(divZ));
	const auto deltaVector = (maxPosition - minPosition) / divideScale;
	auto num = 0;

	for (auto x = minPosition.X; x < maxPosition.X; x += deltaVector.X)
	{
		for (auto y = minPosition.Y; y < maxPosition.Y; y += deltaVector.Y)
		{
			for (auto z = minPosition.Z; z < maxPosition.Z; z += deltaVector.Z)
			{
				const auto min = FVector(x, y, z);
				const auto max = min + deltaVector;
				createData.Add(CreateWaypoint(min, max, num++));
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

FWaypoint ANavigationVolumeActor::CreateWaypoint(FVector minLocation, FVector maxLocation, int32 ID)
{
	return FWaypoint{ ID, EWaypointState::None, 0.0f, TArray<int32>(), -1, minLocation, maxLocation };
}

void ANavigationVolumeActor::CreateOctree(int32 recursionIndex, TArray<FWaypoint>& createData)
{

}

CreateOctreeWorker::CreateOctreeWorker(FCriticalSection& mutex, TArray<TArray<FWaypoint>>& data, int32 recursionIndex)
{
	this->mutex = &mutex;
	waypointData = &data;
	this->recursionIndex = recursionIndex;
}

uint32 CreateOctreeWorker::Run()
{

	return 0;
}

void CreateOctreeWorker::Stop()
{
	stopTaskCounter.Increment();
}
