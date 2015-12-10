// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyoPluginObject.generated.h"


/**
 * Example UStruct declared in a plugin module
 */
USTRUCT()
struct FMyoPluginStruct
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY()
	FString TestString;
};
 

/**
 * Example of declaring a UObject in a plugin module
 */
UCLASS()
class MYOPLUGIN_API UMyoPluginObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:

private:

	UPROPERTY()
	FMyoPluginStruct MyStruct;

};


