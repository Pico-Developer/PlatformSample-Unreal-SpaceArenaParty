// Copyright Epic Games, Inc. All Rights Reserved.

#include "PXR_PluginWrapper.h"
#include "PXR_HMDModule.h"

#if PLATFORM_ANDROID
#include <dlfcn.h>
#endif

DEFINE_LOG_CATEGORY(LogPICOPluginWrapper);

static void* LoadEntryPoint(void* handle, const char* EntryPointName);

bool InitializePICOPluginWrapper(PICOPluginWrapper* wrapper)
{
	if (wrapper->Initialized)
	{
		UE_LOG(LogPICOPluginWrapper, Warning, TEXT("wrapper already initialized"));
		return true;
	}

#if PICO_HMD_SUPPORTED_PLATFORMS
	void* LibraryHandle = nullptr;

#if PLATFORM_ANDROID
	const bool VersionValid = FAndroidMisc::GetAndroidBuildVersion() > 23;
#else
	const bool VersionValid = true;
#endif

	if (VersionValid)
	{
		LibraryHandle = FPICOXRHMDModule::GetPVRPluginHandle();
		if (LibraryHandle == nullptr)
		{
			UE_LOG(LogPICOPluginWrapper, Warning, TEXT("GetPVRPluginHandle() returned NULL"));
			return false;
		}
	}
	else
	{
		return false;
	}
#else
	return false;
#endif

	struct PICOEntryPoint
	{
		const char* EntryPointName;
		void** EntryPointPtr;
	};

#define PICO_BIND_ENTRY_POINT(Func)	{ "Pxr_"#Func, (void**)&wrapper->Func }

	PICOEntryPoint entryPointArray[] =
	{
		// PXRPlugin.h
		PICO_BIND_ENTRY_POINT(SetGraphicOption),
		PICO_BIND_ENTRY_POINT(SetPlatformOption),
		PICO_BIND_ENTRY_POINT(IsInitialized),
		PICO_BIND_ENTRY_POINT(SetInitializeData),
		PICO_BIND_ENTRY_POINT(Initialize),
		PICO_BIND_ENTRY_POINT(Shutdown),
		PICO_BIND_ENTRY_POINT(GetDeviceExtensionsVk),
		PICO_BIND_ENTRY_POINT(GetInstanceExtensionsVk),
		PICO_BIND_ENTRY_POINT(CreateVulkanSystem),
		PICO_BIND_ENTRY_POINT(GetFeatureSupported),
		PICO_BIND_ENTRY_POINT(CreateLayer),
		PICO_BIND_ENTRY_POINT(GetLayerImageCount),
		PICO_BIND_ENTRY_POINT(GetLayerImage),
		PICO_BIND_ENTRY_POINT(GetLayerNextImageIndex),
		PICO_BIND_ENTRY_POINT(GetLayerFoveationImage),
		PICO_BIND_ENTRY_POINT(DestroyLayer),
		PICO_BIND_ENTRY_POINT(IsRunning),
		PICO_BIND_ENTRY_POINT(BeginXr),
		PICO_BIND_ENTRY_POINT(EndXr),
		PICO_BIND_ENTRY_POINT(GetPredictedDisplayTime),
		PICO_BIND_ENTRY_POINT(GetPredictedMainSensorState),
		PICO_BIND_ENTRY_POINT(GetPredictedMainSensorState2),
		PICO_BIND_ENTRY_POINT(GetPredictedMainSensorStateWithEyePose),
		PICO_BIND_ENTRY_POINT(ResetSensor),
		PICO_BIND_ENTRY_POINT(WaitFrame),
		PICO_BIND_ENTRY_POINT(BeginFrame),
		PICO_BIND_ENTRY_POINT(SubmitLayer),
		PICO_BIND_ENTRY_POINT(SubmitLayer2),
		PICO_BIND_ENTRY_POINT(EndFrame),
		PICO_BIND_ENTRY_POINT(PollEvent),
		PICO_BIND_ENTRY_POINT(LogPrint),
		PICO_BIND_ENTRY_POINT(GetFov),
		PICO_BIND_ENTRY_POINT(GetFrustum),
		PICO_BIND_ENTRY_POINT(SetPerformanceLevels),
		PICO_BIND_ENTRY_POINT(GetPerformanceLevels),
		PICO_BIND_ENTRY_POINT(SetColorSpace),
		PICO_BIND_ENTRY_POINT(GetFoveationLevel),
		PICO_BIND_ENTRY_POINT(SetFoveationLevel),
		PICO_BIND_ENTRY_POINT(SetFoveationParams),
		PICO_BIND_ENTRY_POINT(SetTrackingMode),
		PICO_BIND_ENTRY_POINT(GetTrackingMode),
		PICO_BIND_ENTRY_POINT(GetEyeTrackingData),
		PICO_BIND_ENTRY_POINT(GetFaceTrackingData),
		PICO_BIND_ENTRY_POINT(SetTrackingOrigin),
		PICO_BIND_ENTRY_POINT(GetTrackingOrigin),
		PICO_BIND_ENTRY_POINT(GetIPD),
		PICO_BIND_ENTRY_POINT(GetEyeOrientation),
		PICO_BIND_ENTRY_POINT(GetAppHasFocus),
		PICO_BIND_ENTRY_POINT(GetConfigInt),
		PICO_BIND_ENTRY_POINT(GetConfigFloat),
		PICO_BIND_ENTRY_POINT(SetConfigFloatArray),
		PICO_BIND_ENTRY_POINT(SetConfigInt),
		PICO_BIND_ENTRY_POINT(SetConfigString),
		PICO_BIND_ENTRY_POINT(SetConfigUint64),
		PICO_BIND_ENTRY_POINT(StartCameraPreview),
		PICO_BIND_ENTRY_POINT(GetDialogState),
		PICO_BIND_ENTRY_POINT(GetBoundaryConfigured),
		PICO_BIND_ENTRY_POINT(GetBoundaryEnabled),
		PICO_BIND_ENTRY_POINT(SetBoundaryVisible),
		PICO_BIND_ENTRY_POINT(SetSeeThroughBackground),
		PICO_BIND_ENTRY_POINT(GetBoundaryVisible),
		PICO_BIND_ENTRY_POINT(TestNodeIsInBoundary),
		PICO_BIND_ENTRY_POINT(TestPointIsInBoundary),
		PICO_BIND_ENTRY_POINT(GetBoundaryGeometry),
		PICO_BIND_ENTRY_POINT(GetBoundaryDimensions),
		PICO_BIND_ENTRY_POINT(SetSeeThroughImageExtent),
		PICO_BIND_ENTRY_POINT(GetSeeThroughData),
		PICO_BIND_ENTRY_POINT(EnableMultiview),
		PICO_BIND_ENTRY_POINT(GetMrcPose),
		PICO_BIND_ENTRY_POINT(SetMrcPose),
		PICO_BIND_ENTRY_POINT(SetIsSupportMovingMrc),
		PICO_BIND_ENTRY_POINT(SetSensorLostCustomMode),
		PICO_BIND_ENTRY_POINT(SetSensorLostCMST),
		PICO_BIND_ENTRY_POINT(GetDisplayRefreshRatesAvailable),
		PICO_BIND_ENTRY_POINT(SetDisplayRefreshRate),
		PICO_BIND_ENTRY_POINT(GetDisplayRefreshRate),
		PICO_BIND_ENTRY_POINT(SetExtraLatencyMode),
		PICO_BIND_ENTRY_POINT(getPsensorState),
		PICO_BIND_ENTRY_POINT(GetControllerCapabilities),
		PICO_BIND_ENTRY_POINT(GetControllerConnectStatus),
		PICO_BIND_ENTRY_POINT(GetControllerTrackingState),
		PICO_BIND_ENTRY_POINT(GetControllerInputState),
		PICO_BIND_ENTRY_POINT(SetControllerVibration),
		PICO_BIND_ENTRY_POINT(SetControllerVibrationEvent),
		PICO_BIND_ENTRY_POINT(SetControllerEnableKey),
		PICO_BIND_ENTRY_POINT(SetControllerMainInputHandle),
		PICO_BIND_ENTRY_POINT(GetControllerMainInputHandle),
		PICO_BIND_ENTRY_POINT(StopControllerVCMotor),
		PICO_BIND_ENTRY_POINT(StartControllerVCMotor),
		PICO_BIND_ENTRY_POINT(SetControllerAmp),
		PICO_BIND_ENTRY_POINT(SetControllerDelay),
		PICO_BIND_ENTRY_POINT(GetVibrateDelayTime),
		PICO_BIND_ENTRY_POINT(StartVibrateBySharemF),
		PICO_BIND_ENTRY_POINT(StartVibrateBySharemU),
		PICO_BIND_ENTRY_POINT(StartVibrateByCache),
		PICO_BIND_ENTRY_POINT(ClearVibrateByCache),
		PICO_BIND_ENTRY_POINT(StartVibrateByPHF),
		PICO_BIND_ENTRY_POINT(PauseVibrate),
		PICO_BIND_ENTRY_POINT(ResumeVibrate),
		PICO_BIND_ENTRY_POINT(UpdateVibrateParams),
		PICO_BIND_ENTRY_POINT(CreateHapticStream),
		PICO_BIND_ENTRY_POINT(WriteHapticStream),
		PICO_BIND_ENTRY_POINT(SetPHFHapticSpeed),
		PICO_BIND_ENTRY_POINT(GetPHFHapticSpeed),
		PICO_BIND_ENTRY_POINT(GetCurrentFrameSequence),
		PICO_BIND_ENTRY_POINT(StartPHFHaptic),
		PICO_BIND_ENTRY_POINT(StopPHFHaptic),
		PICO_BIND_ENTRY_POINT(RemovePHFHaptic),
		PICO_BIND_ENTRY_POINT(SetAppHandTrackingEnabled),
		PICO_BIND_ENTRY_POINT(GetHandTrackerSettingState),
		PICO_BIND_ENTRY_POINT(GetHandTrackerActiveInputType),
		PICO_BIND_ENTRY_POINT(GetHandTrackerJointLocations),
		PICO_BIND_ENTRY_POINT(GetHandTrackerAimState),
		PICO_BIND_ENTRY_POINT(GetHandTrackerAimStateWithPTFG),
		PICO_BIND_ENTRY_POINT(GetHandTrackerJointLocationsWithPTFG),
		PICO_BIND_ENTRY_POINT(GetHandTrackerAimStateWithPT),
		PICO_BIND_ENTRY_POINT(GetHandTrackerJointLocationsWithPT)
	};

#undef PICO_BIND_ENTRY_POINT

	bool result = true;
	for (int i = 0; i < UE_ARRAY_COUNT(entryPointArray); ++i)
	{
		*(entryPointArray[i].EntryPointPtr) = LoadEntryPoint(LibraryHandle, entryPointArray[i].EntryPointName);

		if (*entryPointArray[i].EntryPointPtr == NULL)
		{
			UE_LOG(LogPICOPluginWrapper, Error, TEXT("PICOPlugin EntryPoint could not be loaded: %s"), entryPointArray[i].EntryPointName);
			result = false;
		}
	}

	if (result)
	{
		UE_LOG(LogPICOPluginWrapper, Log, TEXT("PICOPlugin initialized successfully"));
	}
	else
	{
		DestroyPICOPluginWrapper(wrapper);
	}

	return result;
}

void DestroyPICOPluginWrapper(PICOPluginWrapper* wrapper)
{
	if (!wrapper->Initialized)
	{
		return;
	}
	wrapper->Reset();
	UE_LOG(LogPICOPluginWrapper, Log, TEXT("PICOPlugin destroyed successfully"));
}

static void* LoadEntryPoint(void* Handle, const char* EntryPointName)
{
	if (Handle == nullptr)
	{
		return nullptr;
	}
#if PLATFORM_ANDROID
	void* ptr = dlsym(Handle, EntryPointName);
	if (ptr == nullptr)
	{
		UE_LOG(LogPICOPluginWrapper, Error, TEXT("Unable to load entry point: %s, error %s"), ANSI_TO_TCHAR(EntryPointName), ANSI_TO_TCHAR(dlerror()));
	}
	return ptr;
#else
	UE_LOG(LogPICOPluginWrapper, Error, TEXT("LoadEntryPoint: Unsupported platform"));
	return nullptr;
#endif
}