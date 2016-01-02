// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NavigationVolumeActor.generated.h"

UCLASS()
class ALLRANGE_API ANavigationVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NavigationVolume")
	UBoxComponent* BoxVolume;
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
	void DivideVolume(UBoxComponent* volume, int32 divX, int32 divY, int32 divZ);
	
	
};
