#pragma once

#include "GameFramework/PlayerController.h"
#include "MyoInterface.h"
#include "MyoPlayerController.generated.h"

class UMyoComponent;
class UMyoController;
enum class MyoLockingPolicy : uint8;

UCLASS()
class AMyoPlayerController : public APlayerController, public IMyoInterface
{
	GENERATED_BODY()

public:
	UMyoComponent* MyoComponent;

	AMyoPlayerController(const FObjectInitializer& PCIP);

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
