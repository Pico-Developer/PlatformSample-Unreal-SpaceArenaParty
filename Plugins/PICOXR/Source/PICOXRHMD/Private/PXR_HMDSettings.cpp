// Copyright Epic Games, Inc. All Rights Reserved.

#include "PXR_HMDSettings.h"

#if PICO_HMD_SUPPORTED_PLATFORMS

FGameSettings::FGameSettings() :
	BaseOffset(0, 0, 0)
	, BaseOrientation(FQuat::Identity)
	, PixelDensity(1.0f)
	, PixelDensityMin(0.5f)
	, PixelDensityMax(1.0f)
	, FoveatedRenderingLevel(PxrFoveationLevel::PXR_FOVEATION_LEVEL_NONE)
	, bApplyColorScaleAndOffsetToAllLayers(false)
	, ColorScale(PxrVector4f{ 1,1,1,1 })
	, ColorOffset(PxrVector4f{ 0,0,0,0 })
	, bLateLatching(false)
	, bWaitFrameAtGameFrameTail(false)
	, SeeThroughState(0)
{
	Flags.Raw = 0;
	Flags.bHMDEnabled = true;

#if PLATFORM_ANDROID
	Flags.bsRGBEyeBuffer = true;
	Flags.bStereoEnabled = true;
	CurrentShaderPlatform = EShaderPlatform::SP_VULKAN_ES3_1_ANDROID;
#else
	Flags.bsRGBEyeBuffer = false;
	Flags.bStereoEnabled = false;
	CurrentShaderPlatform = EShaderPlatform::SP_PCD3D_SM5;
#endif
	EyeRenderViewport[0] = EyeRenderViewport[1] = FIntRect(0, 0, 0, 0);
	RenderTargetSize = FIntPoint(0, 0);
}

TSharedPtr<FGameSettings, ESPMode::ThreadSafe> FGameSettings::Clone() const
{
	TSharedPtr<FGameSettings, ESPMode::ThreadSafe> NewSettings = MakeShareable(new FGameSettings(*this));
	return NewSettings;
}

void FGameSettings::SetPixelDensity(float NewPixelDensity)
{
	if (Flags.bPixelDensityAdaptive)
	{
		PixelDensity = FMath::Clamp(NewPixelDensity, PixelDensityMin, PixelDensityMax);
	}
	else
	{
		PixelDensity = FMath::Clamp(NewPixelDensity, ClampPixelDensityMin, ClampPixelDensityMax);
	}
}

#endif //PICO_HMD_SUPPORTED_PLATFORMS
