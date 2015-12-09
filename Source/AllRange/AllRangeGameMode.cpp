// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "AllRange.h"
#include "AllRangeGameMode.h"
#include "AllRangeHUD.h"
#include "AllRangeCharacter.h"

AAllRangeGameMode::AAllRangeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAllRangeHUD::StaticClass();
}
