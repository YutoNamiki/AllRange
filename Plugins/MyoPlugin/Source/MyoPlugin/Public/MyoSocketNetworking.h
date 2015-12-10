#pragma once

#include "HAL/ThreadingBase.h"
#include "MyoSocketNetworking.generated.h"

class FSocket;

class UMyoDataCollectWorker : public FRunnable
{
public:

	UMyoDataCollectWorker();
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

private:
	FSocket* socket;

};

UCLASS()
class UMyoSocketNetworking : public UObject
{

	GENERATED_BODY()

public:

	UMyoSocketNetworking(const class FObjectInitializer& ObjectInitializer);
	virtual void BeginDestroy() override;

private:
	FRunnableThread* thread;
	UMyoDataCollectWorker* worker;


};
