// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "PXR_HMDPrivate.h"

#if PICO_HMD_SUPPORTED_PLATFORMS

static const float ClampPixelDensityMin = 0.5f;
static const float ClampPixelDensityMax = 2.0f;

//-------------------------------------------------------------------------------------------------
// FGameSettings
//-------------------------------------------------------------------------------------------------

class FGameSettings : public TSharedFromThis<FGameSettings, ESPMode::ThreadSafe>
{
public:
	union
	{
		struct
		{
			uint64	bStereoEnabled : 1;
			uint64	bHMDEnabled : 1;
			uint64	bPauseRendering : 1;
			uint64	bPixelDensityAdaptive : 1;
			uint64	bsRGBEyeBuffer : 1;
		};
		uint64 Raw;
	} Flags;

	FVector BaseOffset;
	FQuat BaseOrientation;

	FIntRect EyeRenderViewport[2];
	FIntRect EyeUnscaledRenderViewport[2];

	FMatrix EyeProjectionMatrices[2];
	FMatrix MonoProjectionMatrix;

	FIntPoint RenderTargetSize;
	float PixelDensity;
	float PixelDensityMin;
	float PixelDensityMax;

	PxrFoveationLevel FoveatedRenderingLevel;

	bool bApplyColorScaleAndOffsetToAllLayers;
	PxrVector4f ColorScale, ColorOffset;
    EShaderPlatform CurrentShaderPlatform;

	bool bLateLatching;
	bool bWaitFrameAtGameFrameTail;

	int SeeThroughState;
public:
	FGameSettings();
	virtual ~FGameSettings() {}

	bool IsStereoEnabled() const { return Flags.bStereoEnabled && Flags.bHMDEnabled; }

	void SetPixelDensity(float NewPixelDensity);
	
	TSharedPtr<FGameSettings, ESPMode::ThreadSafe> Clone() const;
};

typedef TSharedPtr<FGameSettings, ESPMode::ThreadSafe> FSettingsPtr;

#endif //PICO_HMD_SUPPORTED_PLATFORMS
