// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AllRange : ModuleRules
{
	public AllRange(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MyoPlugin", "WiiRemotePlugin" });
        PrivateDependencyModuleNames.AddRange(new string[] { "MyoPlugin", "WiiRemotePlugin" });
	}
}
