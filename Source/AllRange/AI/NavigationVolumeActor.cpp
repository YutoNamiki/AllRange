// Fill out your copyright notice in the Description page of Project Settings.

#include "AllRange.h"
#include "NavigationVolumeActor.h"


// Sets default values
ANavigationVolumeActor::ANavigationVolumeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
}

void ANavigationVolumeActor::DivideVolume(UBoxComponent * volume, int32 divX, int32 divY, int32 divZ)
{
}

