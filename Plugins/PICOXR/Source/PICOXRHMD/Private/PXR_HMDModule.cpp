//Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc. All rights reserved.

#include "PXR_HMDModule.h"
#include "PXR_HMD.h"
#include "PXR_HMDRuntimeSettings.h"
#include "HardwareInfo.h"
#include "PXR_HMDPrivateRHI.h"
#include "PXR_Log.h"

#if WITH_EDITOR
#include "PropertyEditorModule.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

#define LOCTEXT_NAMESPACE "FPICOXRModule"

//-------------------------------------------------------------------------------------------------
// FPICOXRHMDModule
//-------------------------------------------------------------------------------------------------
PICOPluginWrapper FPICOXRHMDModule::PluginWrapper;

FPICOXRHMDModule::FPICOXRHMDModule()
{
#if PICO_HMD_SUPPORTED_PLATFORMS
	bPreInit = false;
	bPreInitCalled = false;
#endif
}

void FPICOXRHMDModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "PICOXRSetting",
		LOCTEXT("PICOXRSettingsName", "PICOXR Settings"),
		LOCTEXT("PICOXRSettingsDescription", "Configure the PICOXR plug-in."),
		GetMutableDefault<UPICOXRSettings>());
		UPICOXRSettings* const Settings= GetMutableDefault<UPICOXRSettings>();
		Settings->HandlesRGBHWSupport();
	}
#endif
}

void FPICOXRHMDModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "PICOXRSetting");
	}
#endif
}

void FPICOXRHMDModule::StartupModule()
{
	IHeadMountedDisplayModule::StartupModule();
	FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this,&FPICOXRHMDModule::RegisterSettings);
	FString PluginShaderDir = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("PICOXR/Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/PICOXR"), PluginShaderDir);
}

void FPICOXRHMDModule::ShutdownModule()
{
	UnregisterSettings();
#if PICO_HMD_SUPPORTED_PLATFORMS
	if (PluginWrapper.Initialized)
	{
		DestroyPICOPluginWrapper(&PluginWrapper);
	}

	if (PVRPluginHandle)
	{
		FPlatformProcess::FreeDllHandle(PVRPluginHandle);
		PVRPluginHandle = nullptr;
	}
#endif
}

FString FPICOXRHMDModule::GetModuleKeyName() const
{
	return FString(TEXT("PICOXRHMD"));
}

void FPICOXRHMDModule::GetModuleAliases(TArray<FString>& AliasesOut) const
{
	AliasesOut.Add(TEXT("PICOHMD"));
}

bool FPICOXRHMDModule::IsHMDConnected()
{
#if PICO_HMD_SUPPORTED_PLATFORMS
	if (FApp::CanEverRender())
	{
		return true;
	}
#endif // PICO_HMD_SUPPORTED_PLATFORMS
	return false;
}

bool FPICOXRHMDModule::PreInit()
{
	if (!bPreInitCalled)
	{
		bPreInit = false;
		bPreInitCalled = true;

		if (FApp::CanEverRender())
		{
			PVRPluginHandle = GetPVRPluginHandle();

			if (!PVRPluginHandle)
			{
				return false;
			}

			if (!InitializePICOPluginWrapper(&PluginWrapper))
			{
				UE_LOG(LogHMD, Log, TEXT("Failed InitializePICOPluginWrapper"));
				return false;
			}

			PxrInitParamData initParamData;
			bool bPackagedForVulkan = false;
#if PLATFORM_ANDROID
			initParamData.activity = (void*)FAndroidApplication::GetGameActivityThis();
			initParamData.vm = (void*)GJavaVM;
			FConfigFile AndroidEngineSettings;
			FConfigCacheIni::LoadLocalIniFile(AndroidEngineSettings, TEXT("Engine"), true, TEXT("Android"));

			AndroidEngineSettings.GetBool(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("bSupportsVulkan"), bPackagedForVulkan);
			UE_LOG(LogHMD, Log, TEXT("PICO game packaged for Vulkan : %d"), bPackagedForVulkan);

			bool bIsHMD3Dof = false;
			AndroidEngineSettings.GetBool(TEXT("/Script/PICOXRHMD.PICOXRSettings"), TEXT("bIsHMD3Dof"), bIsHMD3Dof);
			UE_LOG(LogHMD, Log, TEXT("PICO game packaged for bIsHMD3Dof : %d"), bIsHMD3Dof);
			initParamData.headdof = bIsHMD3Dof ? 0 : 1;

			bool bIsController3Dof = false;
			AndroidEngineSettings.GetBool(TEXT("/Script/PICOXRHMD.PICOXRSettings"), TEXT("bIsController3Dof"), bIsController3Dof);
			UE_LOG(LogHMD, Log, TEXT("PICO game packaged for bIsController3Dof : %d"), bIsController3Dof);
			initParamData.controllerdof = bIsController3Dof ? 0 : 1;

#else
			initParamData.activity = initParamData.vm = nullptr;
#endif
			FPICOXRHMDModule::GetPluginWrapper().SetInitializeData(&initParamData);
			FPICOXRHMDModule::GetPluginWrapper().SetPlatformOption(PxrPlatformOption::PXR_UNREAL);
			FPICOXRHMDModule::GetPluginWrapper().SetGraphicOption(bPackagedForVulkan ? PxrGraphicOption::PXR_VULKAN : PxrGraphicOption::PXR_OPENGL_ES);

			float ModulePriority;
			if (!GConfig->GetFloat(TEXT("HMDPluginPriority"), *GetModuleKeyName(), ModulePriority, GEngineIni))
			{
				ModulePriority = 45.0f;
				GConfig->SetFloat(TEXT("HMDPluginPriority"), *GetModuleKeyName(), ModulePriority, GEngineIni);
			}

			UE_LOG(LogHMD, Log, TEXT("FPICOXRHMDModule PreInit successfully"));
		}
	}
	return true;
}

FString FPICOXRHMDModule::GetDeviceSystemName()
{
	return FString(TEXT("PICO XR Device"));
}

TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > FPICOXRHMDModule::CreateTrackingSystem()
{
    PXR_LOGI(PxrUnreal, "CreateTrackingSystem");
#if PICO_HMD_SUPPORTED_PLATFORMS
	if (PreInit())
	{
		TSharedPtr< FPICOXRHMD, ESPMode::ThreadSafe > PICOMobileHMD = FSceneViewExtensions::NewExtension<FPICOXRHMD>();

		if (PICOMobileHMD->Startup())
		{
			HeadMountedDisplay = PICOMobileHMD;
			return PICOMobileHMD;
		}
		HeadMountedDisplay = nullptr;
	}
#endif
	return nullptr;
}

TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe > FPICOXRHMDModule::GetVulkanExtensions()
{
    PXR_LOGI(PxrUnreal, "GetVulkanExtensions");
#if PICO_HMD_SUPPORTED_PLATFORMS
	if (PreInit())
	{
		if (!VulkanExtensions.IsValid())
		{
			VulkanExtensions = MakeShareable(new FVulkanExtensions);
		}
	}
	return VulkanExtensions;
#endif
	return nullptr;
}

#if ENGINE_MINOR_VERSION>26
bool FPICOXRHMDModule::IsStandaloneStereoOnlyDevice()
{
#if PLATFORM_ANDROID
	return FAndroidMisc::GetDeviceMake().ToUpper() == FString(TEXT("PICO"));
#else
	return false;
#endif
}
#endif

void* FPICOXRHMDModule::GetPVRPluginHandle()
{
	void* PVRPluginHandle = nullptr;
#if PLATFORM_ANDROID
	PVRPluginHandle = FPlatformProcess::GetDllHandle(TEXT("libpxr_api.so"));
#endif
	return PVRPluginHandle;
}

IMPLEMENT_MODULE(FPICOXRHMDModule, PICOXRHMD)

#undef  LOCTEXT_NAMESPACE