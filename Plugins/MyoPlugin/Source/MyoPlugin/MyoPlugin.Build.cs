// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class MyoPlugin : ModuleRules
	{
		public MyoPlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
                    "MyoPlugin/Classes",
					"MyoPlugin/Public",
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					"MyoPlugin/Private",
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
                    "Engine",
                    "InputCore",
                    "Slate",
                    "SlateCore",
                    "Networking",
                    "Sockets",
				}
				);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
                    
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					
				}
				);
		}
	}
}