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

#include "UObject/Object.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PicoUpdateSessionCallbackProxy.generated.h"

/**
 * Exposes UpdateSession of the Platform SDK for blueprint use.
 */
UCLASS(MinimalAPI)
class UPicoUpdateSessionCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// Called when the session was updated successfully
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	// Called when there was an error updating the session
	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

	// Kick off UpdateSession check. Asynchronous-- see OnUpdateCompleteDelegate for results.
	UFUNCTION(BlueprintCallable, Category = "Pico|Session", meta = (BlueprintInternalUseOnly = "true"))
	static UPicoUpdateSessionCallbackProxy* SetSessionEnqueue(bool bShouldEnqueueInMatchmakingPool);

	/** UOnlineBlueprintCallProxyBase interface */
	virtual void Activate() override;

private:
	// Internal callback when session updates completes
	void OnUpdateCompleted(FName SessionName, bool bWasSuccessful);

	// The delegate executed by the online subsystem
	FOnUpdateSessionCompleteDelegate UpdateCompleteDelegate;

	// Handles to the registered delegates above
	FDelegateHandle UpdateCompleteDelegateHandle;

	bool bShouldEnqueueInMatchmakingPool;

};
