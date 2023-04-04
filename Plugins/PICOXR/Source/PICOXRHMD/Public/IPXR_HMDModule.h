//Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc. All rights reserved.

#pragma once
#include "Modules/ModuleManager.h"
#include "IHeadMountedDisplayModule.h"

#define PICO_HMD_SUPPORTED_PLATFORMS (PLATFORM_WINDOWS && WINVER > 0x0502)  || (PLATFORM_ANDROID_ARM || PLATFORM_ANDROID_ARM64)

class IPICOXRHMDModule : public IHeadMountedDisplayModule
{
public:

	static inline IPICOXRHMDModule& Get()
	{
		return FModuleManager::LoadModuleChecked< IPICOXRHMDModule >("PICOXRHMD");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("PICOXRHMD");
	}

	virtual FString GetDeviceSystemName() = 0;

};
