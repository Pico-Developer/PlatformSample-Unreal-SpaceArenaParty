/*******************************************************************************
Copyright © 2015-2022 PICO Technology Co., Ltd.All rights reserved.

NOTICE：All information contained herein is, and remains the property of
PICO Technology Co., Ltd. The intellectual and technical concepts
contained herein are proprietary to PICO Technology Co., Ltd. and may be
covered by patents, patents in process, and are protected by trade secret or
copyright law. Dissemination of this information or reproduction of this
material is strictly forbidden unless prior written permission is obtained from
PICO Technology Co., Ltd.
*******************************************************************************/
// This plugin incorporates portions of the Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc.In the United States of America and elsewhere.
// Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc.All rights reserved.

using UnrealBuildTool;
using System.IO;
public class OnlineSubsystemPico : ModuleRules
{
    public OnlineSubsystemPico(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDefinitions.Add("ONLINESUBSYSTEMPICO_PACKAGE=1");
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        string OnlineSubsystemPublic = Path.GetFullPath(Path.Combine(Target.RelativeEnginePath, "Plugins/Online/OnlineSubsystem/Source/Public/"));
        PublicIncludePaths.Add(Path.Combine(PluginDirectory, "Source/OnlineSubsystemPico/include"));
        PrivateIncludePaths.Add("OnlineSubsystemPico/Private");
        PrivateIncludePaths.AddRange(
             new string[] {
                OnlineSubsystemPublic
             }
        );
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "Source/OnlineSubsystemPico/lib/armeabi-v7a/libpxrplatformloader.so"));
            PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "Source/OnlineSubsystemPico/lib/arm64-v8a/libpxrplatformloader.so"));
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "OnlineSubsystemPico_APL.xml"));
        }
        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "Source/OnlineSubsystemPico/lib/Windows/platformsdk.lib"));
            // Ensure that the DLL is staged along with the executable

            string DllDirectory = Path.Combine(PluginDirectory, "Source/OnlineSubsystemPico/lib/Windows/");

            RuntimeDependencies.Add("$(BinaryOutputDir)/libEGL.dll", Path.Combine(DllDirectory, "libEGL.dll"));
            RuntimeDependencies.Add("$(BinaryOutputDir)/libGLESv2.dll", Path.Combine(DllDirectory, "libGLESv2.dll"));
            RuntimeDependencies.Add("$(BinaryOutputDir)/openh264-4.dll", Path.Combine(DllDirectory, "openh264-4.dll"));
            RuntimeDependencies.Add("$(BinaryOutputDir)/platformsdk.dll", Path.Combine(DllDirectory, "platformsdk.dll"));
            RuntimeDependencies.Add("$(BinaryOutputDir)/RTCFFmpeg.dll", Path.Combine(DllDirectory, "RTCFFmpeg.dll"));
            RuntimeDependencies.Add("$(BinaryOutputDir)/VolcEngineRTC.dll", Path.Combine(DllDirectory, "VolcEngineRTC.dll"));
            RuntimeDependencies.Add("$(BinaryOutputDir)/pthreadVC2.dll", Path.Combine(DllDirectory, "pthreadVC2.dll"));
        }

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PublicIncludePathModuleNames.Add("Launch");
        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                        "UnrealEd",
                        "Slate",
                        "SlateCore",
                        "EditorStyle",
                        "EditorWidgets",
                        "DesktopWidgets",
                        "PropertyEditor",
                        "SharedSettingsWidgets",
                        "SequencerWidgets"
                }
            );
        }

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "OnlineSubsystemUtils",
                "Projects",
                "NetCore",
                "CoreUObject",
                "Engine",
                "Sockets",
                "OnlineSubsystem",
                "Projects",
                "Json",
                "JsonUtilities",
                "PacketHandler",
                "Voice",
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
				// ... add private dependencies that you statically link with here ...	
			}
            );


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
