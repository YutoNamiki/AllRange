#pragma once

#include "GameFramework/Actor.h"
#include "MyoInterface.h"
#include "MyoPluginActor.generated.h"

class UMyoComponent;
class UMyoController;
enum class MyoLockingPolicy : uint8;

UCLASS()
class AMyoPluginActor : public AActor, public IMyoInterface
{
	GENERATED_BODY()

public:
	UMyoComponent* MyoComponent;

	AMyoPluginActor(const FObjectInitializer& PCIP);
	virtual void BeginPlay() override;

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
