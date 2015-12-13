#pragma once

#include "MyoDelegateBlueprint.h"
#include "MyoComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = "Input Controller", meta = (BlueprintSpawnableComponent))
class MYOPLUGIN_API UMyoComponent : public UActorComponent, public IMyoDelegateBlueprint
{
	GENERATED_BODY()

public:
	UMyoComponent(const FObjectInitializer& init);
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	bool IsHubEnabled();
	UFUNCTION(BlueprintCallable, Category = MyoFunctions)
	void SetLockingPolicy(MyoLockingPolicy policy);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	UMyoController* LeftMyo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	UMyoController* RightMyo();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = MyoFunctions)
	UMyoController* PrimaryMyo();
	UFUNCTION(BlueprintCallable, Category = MyoUtilityFunctions)
	void ConvertToMyoOrientationSpace(FRotator orientation, FRotator& converted);
};
