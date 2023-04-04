using UnrealBuildTool;
using System.IO;

public class PICOXREditor : ModuleRules
{
	public PICOXREditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivatePCHHeaderFile = "Public/PICOXREditor.h";

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Private"));

        PrivateIncludePaths.AddRange(
	        new [] {
		        "PICOXRHMD/Private",
		        "PICOXRInput/Private"
	        });
        
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "InputCore",
                "ApplicationCore",
                "InputDevice",
                "BlueprintGraph",
                "AnimGraph",
				"ToolMenus",
				"PICOXRInput"
			}
			);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"PICOXRHMD",
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
			);
	}
}
