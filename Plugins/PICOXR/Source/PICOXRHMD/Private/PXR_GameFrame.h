﻿//Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "ShowFlags.h"

class FPXRGameFrame : public TSharedFromThis<FPXRGameFrame, ESPMode::ThreadSafe>
{
public:
	uint32 FrameNumber;
	int32 ViewNumber;
	double predictedDisplayTimeMs;
	float WorldToMetersScale;
	FTransform TrackingToWorld;
	FVector Position;
	FQuat Orientation;
	FVector AngularVelocity;
	FVector Acceleration;
	FVector AngularAcceleration;
	FVector Velocity;
	FEngineShowFlags ShowFlags;
	union
	{
		struct
		{
			uint64			bSplashIsShown : 1;
			uint64			bSeeThroughIsShown : 1;
			uint64			bLateUpdateOK : 1;
			uint64			bHasWaited : 1;
		};
		uint64 Raw;
	} Flags;
public:
	FPXRGameFrame();
	TSharedPtr<FPXRGameFrame, ESPMode::ThreadSafe> CloneMyself() const;
};

typedef TSharedPtr<FPXRGameFrame, ESPMode::ThreadSafe> FPXRGameFramePtr;