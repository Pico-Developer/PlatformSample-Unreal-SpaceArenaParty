// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "IPXR_HMDModule.h"
#include "PXR_HMDFunctionLibrary.h"
#include "StereoRendering.h"
#include "HAL/RunnableThread.h"
#include "RHI.h"
#include "Runtime/Launch/Resources/Version.h"
#include <functional>

#if PICO_HMD_SUPPORTED_PLATFORMS
#define PICO_HMD_SUPPORTED_PLATFORMS_D3D11 PLATFORM_WINDOWS
#define PICO_HMD_SUPPORTED_PLATFORMS_D3D12 PLATFORM_WINDOWS
#define PICO_HMD_SUPPORTED_PLATFORMS_OPENGL (PLATFORM_ANDROID)
#define PICO_HMD_SUPPORTED_PLATFORMS_VULKAN (PLATFORM_ANDROID)
#else 
#define PICO_HMD_SUPPORTED_PLATFORMS_D3D11 0
#define PICO_HMD_SUPPORTED_PLATFORMS_D3D12 0
#define PICO_HMD_SUPPORTED_PLATFORMS_OPENGL 0
#define PICO_HMD_SUPPORTED_PLATFORMS_VULKAN 0
#endif // PICO_HMD_SUPPORTED_PLATFORMS

#if PICO_HMD_SUPPORTED_PLATFORMS
#include "PXR_PluginWrapper.h"
#endif // PICO_HMD_SUPPORTED_PLATFORMS

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidPlatformMisc.h"
#endif

	/** Check currently executing from Game thread */
PICOXRHMD_API bool InGameThread();

FORCEINLINE void CheckInGameThread()
{
#if DO_CHECK
	check(InGameThread());
#endif
}


/** Check currently executing from Render thread */
PICOXRHMD_API bool InRenderThread();

FORCEINLINE void CheckInRenderThread()
{
#if DO_CHECK
	check(InRenderThread());
#endif
}

/** Check currently executing from RHI thread */
PICOXRHMD_API bool InRHIThread();

FORCEINLINE void CheckInRHIThread()
{
#if DO_CHECK
	check(InRHIThread());
#endif
}
#if PICO_HMD_SUPPORTED_PLATFORMS
struct FPose
{
	FQuat Orientation;
	FVector Position;

	FPose()
		: Orientation(EForceInit::ForceInit)
		, Position(EForceInit::ForceInit)
	{}

	FPose(const FQuat& InOrientation, const FVector& InPosition) : Orientation(InOrientation), Position(InPosition) {}

	FPose Inverse() const
	{
		FQuat InvOrientation = Orientation.Inverse();
		FVector InvPosition = InvOrientation.RotateVector(-Position);
		return FPose(InvOrientation, InvPosition);
	}

	FPose operator*(const FPose& other) const
	{
		return FPose(Orientation * other.Orientation, Orientation.RotateVector(other.Position) + Position);
	}
};

FORCEINLINE FQuat ToFQuat(const PxrQuaternionf& InQuat)
{
	return FQuat(-InQuat.z, InQuat.x, InQuat.y, -InQuat.w);
}

FORCEINLINE PxrQuaternionf ToPxrQuatf(const FQuat& InQuat)
{
	return PxrQuaternionf{ float(InQuat.Y), float(InQuat.Z), float(-InQuat.X), float(-InQuat.W) };
}

FORCEINLINE FVector ToFVector(const PxrVector3f& InVec)
{
	return FVector(-InVec.z, InVec.x, InVec.y);
}

FORCEINLINE PxrVector3f ToPxrVector3f(const FVector& InVec)
{
	return PxrVector3f{ float(InVec.Y), float(InVec.Z), float(-InVec.X) };
}

FORCEINLINE PxrVector4f LinearColorToPxrVector4f(const FLinearColor& InColor)
{
	return PxrVector4f{ InColor.R, InColor.G, InColor.B, InColor.A };
}

bool ConvertPose_Private(const FPose& InPose, FPose& OutPose, const FQuat BaseOrientation, const FVector BaseOffset, float WorldToMetersScale);

bool ConvertPose_Private(const PxrPosef& InPose, FPose& OutPose, const FQuat BaseOrientation, const FVector BaseOffset, float WorldToMetersScale);

bool ConvertPose_Private(const FPose& InPose, PxrPosef& OutPose, const FQuat BaseOrientation, const FVector BaseOffset, float WorldToMetersScale);

bool IsPICOHMDConnected();
#endif