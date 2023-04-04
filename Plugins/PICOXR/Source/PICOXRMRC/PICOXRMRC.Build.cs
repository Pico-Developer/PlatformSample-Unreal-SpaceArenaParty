
using UnrealBuildTool;
using System.IO;

public class PICOXRMRC : ModuleRules
{
    public PICOXRMRC(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateIncludePathModuleNames.AddRange(
                new string[]
                {
                    "InputDevice",			// For IInputDevice.h
					"HeadMountedDisplay",	// For IMotionController.h
					"ImageWrapper",
                    "Engine"
                });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "InputCore",
                    "Slate",
                    "SlateCore",
                    "RHI",
                    "VulkanRHI",
                    "RenderCore",
                    "MediaAssets",
                    "HeadMountedDisplay",
                    "PICOXRHMD",
                    "PICOXRInput",
                    "PXRPlugin",
            });

        PrivateIncludePaths.AddRange(
            new string[] {
                    "PICOXRHMD/Private",
                    "PICOXRInput/Private",
            });
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");
        }
    }
}

