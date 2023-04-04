// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "PXR_HMDPrivate.h"
#include "IHeadMountedDisplayVulkanExtensions.h"

#if PICO_HMD_SUPPORTED_PLATFORMS
//-------------------------------------------------------------------------------------------------
// FVulkanExtensions
//-------------------------------------------------------------------------------------------------

class FVulkanExtensions : public IHeadMountedDisplayVulkanExtensions, public TSharedFromThis<FVulkanExtensions, ESPMode::ThreadSafe>
{
public:
	FVulkanExtensions() {}
	virtual ~FVulkanExtensions() {}

	// IHeadMountedDisplayVulkanExtensions
	virtual bool GetVulkanInstanceExtensionsRequired(TArray<const ANSICHAR*>& Out) override;
	virtual bool GetVulkanDeviceExtensionsRequired(struct VkPhysicalDevice_T *pPhysicalDevice, TArray<const ANSICHAR*>& Out) override;
};
#endif //PICO_HMD_SUPPORTED_PLATFORMS