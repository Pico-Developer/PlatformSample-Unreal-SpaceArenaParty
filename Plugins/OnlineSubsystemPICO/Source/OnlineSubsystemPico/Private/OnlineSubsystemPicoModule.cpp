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


#include "OnlineSubsystemPicoModule.h"
#include "OnlineSubsystemPicoPrivate.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "OnlinePicoSettings.h"
#include "Misc/ConfigCacheIni.h"

#if WITH_EDITOR
#include "PropertyEditorModule.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "OnlinePicoSettingsCustomization.h"
#endif

#define LOCTEXT_NAMESPACE "OnlinePicoModule"

IMPLEMENT_MODULE(FOnlineSubsystemPicoModule, OnlineSubsystemPico);

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryPico : public IOnlineFactory
{
private:

    /** Singleton instance of the Pico OSS */
    FOnlineSubsystemPicoPtr OnlineSub;

public:

    FOnlineFactoryPico() {}
    virtual ~FOnlineFactoryPico() {}

    IOnlineSubsystemPtr CreateSubsystem(FName InstanceName) override
    {
        if (!OnlineSub.IsValid())
        {
            OnlineSub = MakeShared<FOnlineSubsystemPico, ESPMode::ThreadSafe>(InstanceName);
        }
        if (OnlineSub->IsEnabled())
        {
            if (!OnlineSub->IsInitialized())
            {
                if (!OnlineSub->Init())
                {
                    UE_LOG_ONLINE(Warning, TEXT("Pico SDK failed to initialize"));
                    OnlineSub = nullptr;
                }
            }
            else
            {
                UE_LOG_ONLINE(Log, TEXT("Pico SDK already initialized!"));
            }
        }
        else
        {
            UE_LOG_ONLINE(Warning, TEXT("Pico SDK disabled!"));
            OnlineSub->Shutdown();
            OnlineSub = nullptr;
        }
        return OnlineSub;
    }
};

void FOnlineSubsystemPicoModule::StartupModule()
{
#if WITH_EDITOR
    FString BaseDir = IPluginManager::Get().FindPlugin("OnlineSubsystemPico")->GetBaseDir();
    FString LibraryPath;
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/OnlineSubsystemPico/lib/Windows/libplatformsdk.dll"));
    WindowsLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

    LibgccHandel = FPlatformProcess::GetDllHandle(*(BaseDir + "/Source/OnlineSubsystemPico/lib/Windows/libgcc_s_seh-1.dll"));
    libstdcHandel = FPlatformProcess::GetDllHandle(*(BaseDir + "/Source/OnlineSubsystemPico/lib/Windows/libstdc++-6.dll"));
    LibwinpthreadHandel = FPlatformProcess::GetDllHandle(*(BaseDir + "/Source/OnlineSubsystemPico/lib/Windows/libwinpthread-1.dll"));
#endif

    PicoFactory = new FOnlineFactoryPico();
    PicoFactory->CreateSubsystem(FName(TEXT("Pico")));
    // Create and register our singleton factory with the main online subsystem for easy access
    FOnlineSubsystemModule& OnlineSubsystemModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
    OnlineSubsystemModule.RegisterPlatformService(PICO_SUBSYSTEM, PicoFactory);
    FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &FOnlineSubsystemPicoModule::RegisterSettings);
}

void FOnlineSubsystemPicoModule::ShutdownModule()
{
    UE_LOG_ONLINE(Log, TEXT("Pico Module Shutdown"));

    FOnlineSubsystemModule& OnlineSubsystemModule = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
    OnlineSubsystemModule.UnregisterPlatformService(PICO_SUBSYSTEM);
    delete PicoFactory;
    PicoFactory = nullptr;
    UnregisterSettings();

#if WITH_EDITOR
    FPlatformProcess::FreeDllHandle(WindowsLibraryHandle);
    WindowsLibraryHandle = nullptr;

    if (LibgccHandel)
    {
        FPlatformProcess::FreeDllHandle(LibgccHandel);
        LibgccHandel = nullptr;
    }
    if (libstdcHandel)
    {
        FPlatformProcess::FreeDllHandle(libstdcHandel);
        libstdcHandel = nullptr;
    }
    if (LibwinpthreadHandel)
    {
        FPlatformProcess::FreeDllHandle(LibwinpthreadHandel);
        LibwinpthreadHandel = nullptr;
    }
#endif
}

void FOnlineSubsystemPicoModule::RegisterSettings()
{
#if WITH_EDITOR

    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.RegisterCustomClassLayout(
        UOnlinePicoSettings::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FOnlinePicoSettingsCustomization::MakeInstance)
    );
    PropertyModule.NotifyCustomizationModuleChanged();
    // register settings
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

    if (SettingsModule != nullptr)
    {
        ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "OnlinePicoSetting",
            LOCTEXT("OnlinePicoSettingsName", "OnlinePico Settings"),
            LOCTEXT("OnlinePicoSettingsDescription", "Configure the OnlineSubsystemPico plugin."),
            GetMutableDefault<UOnlinePicoSettings>()
        );
    }
#endif
}

void FOnlineSubsystemPicoModule::UnregisterSettings()
{
#if WITH_EDITOR
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

    if (SettingsModule != nullptr)
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "OnlinePicoSetting");
    }
#endif
}
#undef LOCTEXT_NAMESPACE