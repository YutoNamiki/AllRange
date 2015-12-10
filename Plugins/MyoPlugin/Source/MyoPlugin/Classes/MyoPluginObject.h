// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "MyoPluginObject.generated.h"

USTRUCT()
struct FMyoPluginStruct
{
	GENERATED_USTRUCT_BODY()
 
	UPROPERTY()
	FString TestString;
};

UCLASS()
class MYOPLUGIN_API UMyoPluginObject : public UObject
{
	GENERATED_UCLASS_BODY()

public:

private:

	UPROPERTY()
	FMyoPluginStruct MyStruct;

};
