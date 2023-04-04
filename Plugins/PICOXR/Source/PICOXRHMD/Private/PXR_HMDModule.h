// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "PXR_HMDPrivate.h"
#include "IHeadMountedDisplay.h"
#include "PXR_VulkanExtensions.h"
#include "PXR_PluginWrapper.h"

//-------------------------------------------------------------------------------------------------
// FPICOXRHMDModule
//-------------------------------------------------------------------------------------------------
class FPICOXRHMDModule : public IPICOXRHMDModule
{
public:
	FPICOXRHMDModule();
	static inline FPICOXRHMDModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FPICOXRHMDModule >("PICOXRHMD");
	}

	// IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void RegisterSettings();
	void UnregisterSettings();

	// IHeadMountedDisplayModule
	virtual FString GetModuleKeyName() const override;
	virtual void GetModuleAliases(TArray<FString>& AliasesOut) const override;
	virtual bool IsHMDConnected() override;
	virtual bool PreInit() override;
	virtual FString GetDeviceSystemName() override;
    virtual TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > CreateTrackingSystem() override;
    virtual TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe > GetVulkanExtensions() override;
#if ENGINE_MINOR_VERSION>26
	virtual bool IsStandaloneStereoOnlyDevice() override;
#endif

#if PICO_HMD_SUPPORTED_PLATFORMS
public:
	PICOXRHMD_API static void* GetPVRPluginHandle();
	PICOXRHMD_API static inline PICOPluginWrapper& GetPluginWrapper() { return PluginWrapper; }
protected:
	static PICOPluginWrapper PluginWrapper;

	bool bPreInit;
	bool bPreInitCalled;
	void* PVRPluginHandle;
	TWeakPtr< IHeadMountedDisplay, ESPMode::ThreadSafe > HeadMountedDisplay;
	TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe > VulkanExtensions;
#endif
};
