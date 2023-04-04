// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include <memory.h>

#if PLATFORM_SUPPORTS_PRAGMA_PACK
#pragma pack (push,8)
#endif

#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#endif

#pragma warning(push)
#pragma warning(disable:4201)		// nonstandard extension used: nameless struct/union
//#pragma warning(disable:4668)		// 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'

#define PVRP_EXPORT typedef
#include "PXR_Plugin.h"
#undef PVRP_EXPORT
#pragma warning(pop)

#if PLATFORM_WINDOWS
#include "Windows/HideWindowsPlatformTypes.h"
#endif

#if PLATFORM_SUPPORTS_PRAGMA_PACK
#pragma pack (pop)
#endif

#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogPICOPluginWrapper, Log, All);

#define PICO_DECLARE_ENTRY_POINT(Func)		Pxr_##Func* Func

struct PICOPluginWrapper
{
	PICOPluginWrapper()
	{
		Reset();
	}

	void Reset()
	{
		memset(this, 0, sizeof(PICOPluginWrapper));
	}

	bool Initialized;

	// PXRPlugin.h
	PICO_DECLARE_ENTRY_POINT(SetGraphicOption);
	PICO_DECLARE_ENTRY_POINT(SetPlatformOption);
	PICO_DECLARE_ENTRY_POINT(IsInitialized);
	PICO_DECLARE_ENTRY_POINT(SetInitializeData);
	PICO_DECLARE_ENTRY_POINT(Initialize);
	PICO_DECLARE_ENTRY_POINT(Shutdown);
	PICO_DECLARE_ENTRY_POINT(GetDeviceExtensionsVk);
	PICO_DECLARE_ENTRY_POINT(GetInstanceExtensionsVk);
	PICO_DECLARE_ENTRY_POINT(CreateVulkanSystem);
	PICO_DECLARE_ENTRY_POINT(GetFeatureSupported);
	PICO_DECLARE_ENTRY_POINT(CreateLayer);
	PICO_DECLARE_ENTRY_POINT(GetLayerImageCount);
	PICO_DECLARE_ENTRY_POINT(GetLayerImage);
	PICO_DECLARE_ENTRY_POINT(GetLayerNextImageIndex);
	PICO_DECLARE_ENTRY_POINT(GetLayerFoveationImage);
	PICO_DECLARE_ENTRY_POINT(DestroyLayer);
	PICO_DECLARE_ENTRY_POINT(IsRunning);
	PICO_DECLARE_ENTRY_POINT(BeginXr);
	PICO_DECLARE_ENTRY_POINT(EndXr);
	PICO_DECLARE_ENTRY_POINT(GetPredictedDisplayTime);
	PICO_DECLARE_ENTRY_POINT(GetPredictedMainSensorState);
	PICO_DECLARE_ENTRY_POINT(GetPredictedMainSensorState2);
	PICO_DECLARE_ENTRY_POINT(GetPredictedMainSensorStateWithEyePose);
	PICO_DECLARE_ENTRY_POINT(ResetSensor);
	PICO_DECLARE_ENTRY_POINT(WaitFrame);
	PICO_DECLARE_ENTRY_POINT(BeginFrame);
	PICO_DECLARE_ENTRY_POINT(SubmitLayer);
	PICO_DECLARE_ENTRY_POINT(SubmitLayer2);
	PICO_DECLARE_ENTRY_POINT(EndFrame);
	PICO_DECLARE_ENTRY_POINT(PollEvent);
	PICO_DECLARE_ENTRY_POINT(LogPrint);
	PICO_DECLARE_ENTRY_POINT(GetFov);
	PICO_DECLARE_ENTRY_POINT(GetFrustum);
	PICO_DECLARE_ENTRY_POINT(SetPerformanceLevels);
	PICO_DECLARE_ENTRY_POINT(GetPerformanceLevels);
	PICO_DECLARE_ENTRY_POINT(SetColorSpace);
	PICO_DECLARE_ENTRY_POINT(GetFoveationLevel);
	PICO_DECLARE_ENTRY_POINT(SetFoveationLevel);
	PICO_DECLARE_ENTRY_POINT(SetFoveationParams);
	PICO_DECLARE_ENTRY_POINT(SetTrackingMode);
	PICO_DECLARE_ENTRY_POINT(GetTrackingMode);
	PICO_DECLARE_ENTRY_POINT(GetEyeTrackingData);
	PICO_DECLARE_ENTRY_POINT(GetFaceTrackingData);
	PICO_DECLARE_ENTRY_POINT(SetTrackingOrigin);
	PICO_DECLARE_ENTRY_POINT(GetTrackingOrigin);
	PICO_DECLARE_ENTRY_POINT(GetIPD);
	PICO_DECLARE_ENTRY_POINT(GetEyeOrientation);
	PICO_DECLARE_ENTRY_POINT(GetAppHasFocus);
	PICO_DECLARE_ENTRY_POINT(GetConfigInt);
	PICO_DECLARE_ENTRY_POINT(GetConfigFloat);
	PICO_DECLARE_ENTRY_POINT(SetConfigFloatArray);
	PICO_DECLARE_ENTRY_POINT(SetConfigInt);
	PICO_DECLARE_ENTRY_POINT(SetConfigString);
	PICO_DECLARE_ENTRY_POINT(SetConfigUint64);
	PICO_DECLARE_ENTRY_POINT(StartCameraPreview);
	PICO_DECLARE_ENTRY_POINT(GetDialogState);
	PICO_DECLARE_ENTRY_POINT(GetBoundaryConfigured);
	PICO_DECLARE_ENTRY_POINT(GetBoundaryEnabled);
	PICO_DECLARE_ENTRY_POINT(SetBoundaryVisible);
	PICO_DECLARE_ENTRY_POINT(SetSeeThroughBackground);
	PICO_DECLARE_ENTRY_POINT(GetBoundaryVisible);
	PICO_DECLARE_ENTRY_POINT(TestNodeIsInBoundary);
	PICO_DECLARE_ENTRY_POINT(TestPointIsInBoundary);
	PICO_DECLARE_ENTRY_POINT(GetBoundaryGeometry);
	PICO_DECLARE_ENTRY_POINT(GetBoundaryDimensions);
	PICO_DECLARE_ENTRY_POINT(SetSeeThroughImageExtent);
	PICO_DECLARE_ENTRY_POINT(GetSeeThroughData);
	PICO_DECLARE_ENTRY_POINT(EnableMultiview);
	PICO_DECLARE_ENTRY_POINT(GetMrcPose);
	PICO_DECLARE_ENTRY_POINT(SetMrcPose);
	PICO_DECLARE_ENTRY_POINT(SetIsSupportMovingMrc);
	PICO_DECLARE_ENTRY_POINT(SetSensorLostCustomMode);
	PICO_DECLARE_ENTRY_POINT(SetSensorLostCMST);
	PICO_DECLARE_ENTRY_POINT(GetDisplayRefreshRatesAvailable);
	PICO_DECLARE_ENTRY_POINT(SetDisplayRefreshRate);
	PICO_DECLARE_ENTRY_POINT(GetDisplayRefreshRate);
	PICO_DECLARE_ENTRY_POINT(SetExtraLatencyMode);
	PICO_DECLARE_ENTRY_POINT(getPsensorState);
	PICO_DECLARE_ENTRY_POINT(GetControllerCapabilities);					
	PICO_DECLARE_ENTRY_POINT(GetControllerConnectStatus);				
	PICO_DECLARE_ENTRY_POINT(GetControllerTrackingState);			
	PICO_DECLARE_ENTRY_POINT(GetControllerInputState);		
	PICO_DECLARE_ENTRY_POINT(SetControllerVibration);
	PICO_DECLARE_ENTRY_POINT(SetControllerVibrationEvent);				
	PICO_DECLARE_ENTRY_POINT(SetControllerEnableKey);				
	PICO_DECLARE_ENTRY_POINT(SetControllerMainInputHandle);				
	PICO_DECLARE_ENTRY_POINT(GetControllerMainInputHandle);						
	PICO_DECLARE_ENTRY_POINT(StopControllerVCMotor);
	PICO_DECLARE_ENTRY_POINT(StartControllerVCMotor);
	PICO_DECLARE_ENTRY_POINT(SetControllerAmp);
	PICO_DECLARE_ENTRY_POINT(SetControllerDelay);
	PICO_DECLARE_ENTRY_POINT(GetVibrateDelayTime);
	PICO_DECLARE_ENTRY_POINT(StartVibrateBySharemF);
	PICO_DECLARE_ENTRY_POINT(StartVibrateBySharemU);
	PICO_DECLARE_ENTRY_POINT(StartVibrateByCache);
	PICO_DECLARE_ENTRY_POINT(ClearVibrateByCache);
	PICO_DECLARE_ENTRY_POINT(StartVibrateByPHF);
	PICO_DECLARE_ENTRY_POINT(PauseVibrate);
	PICO_DECLARE_ENTRY_POINT(ResumeVibrate);
	PICO_DECLARE_ENTRY_POINT(UpdateVibrateParams);	
	PICO_DECLARE_ENTRY_POINT(CreateHapticStream);
	PICO_DECLARE_ENTRY_POINT(WriteHapticStream);
	PICO_DECLARE_ENTRY_POINT(SetPHFHapticSpeed);
	PICO_DECLARE_ENTRY_POINT(GetPHFHapticSpeed);
	PICO_DECLARE_ENTRY_POINT(GetCurrentFrameSequence);
	PICO_DECLARE_ENTRY_POINT(StartPHFHaptic);
	PICO_DECLARE_ENTRY_POINT(StopPHFHaptic);
	PICO_DECLARE_ENTRY_POINT(RemovePHFHaptic);
	PICO_DECLARE_ENTRY_POINT(SetAppHandTrackingEnabled);				
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerSettingState);
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerActiveInputType);
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerJointLocations);
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerAimState);
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerAimStateWithPTFG);
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerJointLocationsWithPTFG);
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerAimStateWithPT);
	PICO_DECLARE_ENTRY_POINT(GetHandTrackerJointLocationsWithPT);
};

#undef PICO_DECLARE_ENTRY_POINT

bool InitializePICOPluginWrapper(PICOPluginWrapper* wrapper);
void DestroyPICOPluginWrapper(PICOPluginWrapper* wrapper);