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
#include "FindSessionsCallbackProxy.h"
#include "PicoFindSessionsCallbackProxy.generated.h"

/**
 * Exposes FindSession of the Platform SDK for blueprint use.
 */
UCLASS(MinimalAPI)
class UPicoFindSessionsCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_UCLASS_BODY()

	// Called when there is a successful query
	UPROPERTY(BlueprintAssignable)
	FBlueprintFindSessionsResultDelegate OnSuccess;

	// Called when there is an unsuccessful query
	UPROPERTY(BlueprintAssignable)
	FBlueprintFindSessionsResultDelegate OnFailure;

	// Searches for matchmaking room sessions with the pico online subsystem
	UFUNCTION(BlueprintCallable, Category = "Pico|Session", meta = (BlueprintInternalUseOnly = "true"))
	static UPicoFindSessionsCallbackProxy* FindMatchmakingSessions(int32 MaxResults, FString PicoMatchmakingPool);

	// Searches for moderated room sessions with the pico online subsystem
	UFUNCTION(BlueprintCallable, Category = "Pico|Session", meta = (BlueprintInternalUseOnly = "true"))
	static UPicoFindSessionsCallbackProxy* FindModeratedSessions(int32 MaxResults, int PageIndex, int PageSize);

	// UOnlineBlueprintCallProxyBase interface
	virtual void Activate() override;
	// End of UOnlineBlueprintCallProxyBase interface

private:
	// Internal callback when the session search completes, calls out to the public success/failure callbacks
	void OnCompleted(bool bSuccess);

private:

	// The delegate executed by the online subsystem
	FOnFindSessionsCompleteDelegate Delegate;

	// Handle to the registered OnFindSessionsComplete delegate
	FDelegateHandle DelegateHandle;

	// Object to track search results
	TSharedPtr<FOnlineSessionSearch> SearchObject;

	// Maximum number of results to return
	int MaxResults;

	// Optional: if searching within a matchmaking pool
	FString PicoPool;

	int PageIndex;

	int PageSize;

	bool bSearchModeratedRoomsOnly;
};
