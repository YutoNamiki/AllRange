#pragma once

#include "GameFramework/Actor.h"
#include "MyoDelegateBlueprint.h"
#include "MyoInterface.h"
#include "MyoPluginActor.generated.h"

UCLASS()
class AMyoPluginActor : public AActor, public IMyoDelegateBlueprint, public IMyoInterface
{
	GENERATED_BODY()

public:
	AMyoPluginActor(const FObjectInitializer& PCIP);
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	bool IsHubEnabled();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void SetLockingPolicy(MyoLockingPolicy policy);
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	UMyoController* LeftMyo();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	UMyoController* RightMyo();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	UMyoController* PrimaryMyo();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted);

};
