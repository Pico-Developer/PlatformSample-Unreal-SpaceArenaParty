//Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc. All rights reserved.

using UnrealBuildTool;
using System.IO;

public class PICOXRInput : ModuleRules
{
    public PICOXRInput(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateIncludePathModuleNames.AddRange(
                new[]
                {
                    "InputDevice",			// For IInputDevice.h
					"HeadMountedDisplay",	// For IMotionController.h
					"ImageWrapper",
                });

        PrivateDependencyModuleNames.AddRange(
            new[]
            {
                    "Core",
                    "CoreUObject",
                    "ApplicationCore",
                    "Engine",
                    "InputCore",
                    "HeadMountedDisplay",
                    "PICOXRHMD",
                    "PXRPlugin",
            });
        PrivateIncludePaths.AddRange(
                new[] {
                    "PICOXRInput/Private",
                    "PICOXRHMD/Private",
                });

    }
}
