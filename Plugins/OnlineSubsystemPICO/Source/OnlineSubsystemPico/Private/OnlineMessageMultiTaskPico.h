/*******************************************************************************
Copyright © 2015-2022 PICO Technology Co., Ltd.All rights reserved.

NOTICE：All information contained herein is, and remains the property of
PICO Technology Co., Ltd. The intellectual and technical concepts
contained herein are proprietary to PICO Technology Co., Ltd. and may be
covered by patents, patents in process, and are protected by trade secret or
copyright law. Dissemination of this information or reproduction of this
material is strictly forbidden unless prior written permission is obtained from
PICO Technology Co., Ltd.
*******************************************************************************/
// This plugin incorporates portions of the Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc.In the United States of America and elsewhere.
// Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemPicoPrivate.h"
#include "OnlineDelegateMacros.h"
#include "OnlineMessageTaskManagerPico.h"
#include "OnlineSubsystemPicoPackage.h"

/**
 *
 */
class FOnlineMessageMultiTaskPico
{
private:
	/** Requests that are waiting to be completed */
	TArray<ppfRequest> InProgressRequests;

protected:
	bool bDidAllRequestsFinishedSuccessfully = true;

	DECLARE_DELEGATE(FFinalizeDelegate);

	FOnlineMessageMultiTaskPico::FFinalizeDelegate Delegate;

PACKAGE_SCOPE:
	FOnlineSubsystemPico& PicoSubsystem;

	FOnlineMessageMultiTaskPico(
		FOnlineSubsystemPico& InPicoSubsystem,
		const FOnlineMessageMultiTaskPico::FFinalizeDelegate& InDelegate)
		: Delegate(InDelegate)
		, PicoSubsystem(InPicoSubsystem)
	{
	}

	void AddNewRequest(ppfRequest RequestId);
};
