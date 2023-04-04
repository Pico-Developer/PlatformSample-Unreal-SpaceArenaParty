// Copyright Epic Games, Inc. All Rights Reserved.

#include "PXR_HMDPrivate.h"
#include "RHICommandList.h"
#include "RenderingThread.h"

bool InGameThread()
{
	if (GIsGameThreadIdInitialized)
	{
		return FPlatformTLS::GetCurrentThreadId() == GGameThreadId;
	}
	else
	{
		return true;
	}
}


bool InRenderThread()
{
	if (GIsThreadedRendering && !GIsRenderingThreadSuspended.Load(EMemoryOrder::Relaxed))
	{
		return IsInActualRenderingThread();
	}
	else
	{
		return InGameThread();
	}
}


bool InRHIThread()
{
	if (GIsThreadedRendering && !GIsRenderingThreadSuspended.Load(EMemoryOrder::Relaxed))
	{
#if ENGINE_MINOR_VERSION > 25
		if (IsRHIThreadRunning())
#else
		if (GRHIThreadId)
#endif
		{
			if (IsInRHIThread())
			{
				return true;
			}

			if (IsInActualRenderingThread())
			{
				return GetImmediateCommandList_ForRenderCommand().Bypass();
			}

			return false;
		}
		else
		{
			return IsInActualRenderingThread();
		}
	}
	else
	{
		return InGameThread();
	}
}

bool ConvertPose_Private(const FPose& InPose, FPose& OutPose, const FQuat BaseOrientation, const FVector BaseOffset, float WorldToMetersScale)
{
	OutPose.Orientation = BaseOrientation.Inverse() * InPose.Orientation;
	OutPose.Orientation.Normalize();

	OutPose.Position = (InPose.Position - BaseOffset) * WorldToMetersScale;
	OutPose.Position = BaseOrientation.Inverse().RotateVector(OutPose.Position);

	return true;
}

bool ConvertPose_Private(const PxrPosef& InPose, FPose& OutPose, const FQuat BaseOrientation, const FVector BaseOffset, float WorldToMetersScale)
{
	return ConvertPose_Private(FPose(ToFQuat(InPose.orientation), ToFVector(InPose.position)), OutPose, BaseOrientation, BaseOffset, WorldToMetersScale);
}

bool ConvertPose_Private(const FPose& InPose, PxrPosef& OutPose, const FQuat BaseOrientation, const FVector BaseOffset, float WorldToMetersScale)
{
	OutPose.orientation = ToPxrQuatf(BaseOrientation * InPose.Orientation);
	OutPose.position = ToPxrVector3f(BaseOrientation.RotateVector(InPose.Position) / WorldToMetersScale + BaseOffset);
	return true;
}

bool IsPICOHMDConnected()
{
#if PLATFORM_WINDOWS
	return false;
#endif
	return true;
}
