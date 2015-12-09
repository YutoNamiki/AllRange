// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "WiiRemotePluginObject.generated.h"


/**
 * Example UStruct declared in a plugin module
 */
USTRUCT()
struct FWiiRemotePluginStruct
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY()
	FString TestString;
};
 

/**
 * Example of declaring a UObject in a plugin module
 */
UCLASS()
class UWiiRemotePluginObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:

private:

	UPROPERTY()
	FWiiRemotePluginStruct MyStruct;

};


