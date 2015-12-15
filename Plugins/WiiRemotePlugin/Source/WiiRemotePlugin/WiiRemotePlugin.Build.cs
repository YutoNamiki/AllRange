// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class WiiRemotePlugin : ModuleRules
	{
        private string ModulePath
        {
            get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
        }

        private string ThirdPartyPath
        {
            get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
        }

        public WiiRemotePlugin(TargetInfo Target)
		{
			PublicIncludePaths.AddRange(
				new string[] {
                    "WiiRemotePlugin/Public",
					"WiiRemotePlugin/Classes",
				}
				);

            PrivateIncludePaths.AddRange(
                new string[] {
                    "WiiRemotePlugin/Private",

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

            LoadWiiRemoteLib(Target);
		}

        public bool LoadWiiRemoteLib(TargetInfo Target)
        {
            bool isLibrarySupported = false;
            if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
            {
                isLibrarySupported = true;
                string LibrariesPath = Path.Combine(ThirdPartyPath, "Lib");
                PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "WiiRemoteLibrary.lib"));
            }
            if (isLibrarySupported)
                PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Include"));
            Definitions.Add(string.Format("WITH_WIIREMOTE_BINDING={0}", isLibrarySupported ? 1 : 0));
            return isLibrarySupported;
        }
    }
}
