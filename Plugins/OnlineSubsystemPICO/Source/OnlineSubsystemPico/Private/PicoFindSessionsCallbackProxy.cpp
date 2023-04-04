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

#include "PicoFindSessionsCallbackProxy.h"
#include "OnlineSubsystemPicoPrivate.h"
#include "Online.h"
#include "OnlineSessionInterfacePico.h"
#include "OnlineSubsystemPicoPrivate.h"

UPicoFindSessionsCallbackProxy::UPicoFindSessionsCallbackProxy(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , Delegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
    , MaxResults(0)
    , bSearchModeratedRoomsOnly(false)
{
}

UPicoFindSessionsCallbackProxy* UPicoFindSessionsCallbackProxy::FindMatchmakingSessions(int32 MaxResults, FString PicoMatchmakingPool)
{
    UPicoFindSessionsCallbackProxy* Proxy = NewObject<UPicoFindSessionsCallbackProxy>();
    Proxy->SetFlags(RF_StrongRefOnFrame);
    Proxy->MaxResults = MaxResults;
    Proxy->PicoPool = MoveTemp(PicoMatchmakingPool);
    Proxy->bSearchModeratedRoomsOnly = false;
    return Proxy;
}

UPicoFindSessionsCallbackProxy* UPicoFindSessionsCallbackProxy::FindModeratedSessions(int32 MaxResults, int PageIndex, int PageSize)
{
    UPicoFindSessionsCallbackProxy* Proxy = NewObject<UPicoFindSessionsCallbackProxy>();
    Proxy->SetFlags(RF_StrongRefOnFrame);
    Proxy->MaxResults = MaxResults;
    Proxy->bSearchModeratedRoomsOnly = true;
    Proxy->PageIndex = PageIndex;
    Proxy->PageSize = PageSize;
    return Proxy;
}

void UPicoFindSessionsCallbackProxy::Activate()
{
    auto PicoSessionInterface = Online::GetSessionInterface(PICO_SUBSYSTEM);

    if (PicoSessionInterface.IsValid())
    {
        DelegateHandle = PicoSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(Delegate);

        SearchObject = MakeShareable(new FOnlineSessionSearch);
        SearchObject->MaxSearchResults = MaxResults;
        SearchObject->QuerySettings.Set(SEARCH_PICO_MODERATED_ROOMS_ONLY, bSearchModeratedRoomsOnly, EOnlineComparisonOp::Equals);
        SearchObject->QuerySettings.Set(GET_MODERATEDROOMS_PAGEINDEX, PageIndex, EOnlineComparisonOp::Equals);
        SearchObject->QuerySettings.Set(GET_MODERATEDROOMS_PAGESIZE, PageSize, EOnlineComparisonOp::Equals);

        if (!PicoPool.IsEmpty())
        {
            SearchObject->QuerySettings.Set(SETTING_PICO_POOL, PicoPool, EOnlineComparisonOp::Equals);
        }

        PicoSessionInterface->FindSessions(0, SearchObject.ToSharedRef());
    }
    else
    {
        UE_LOG_ONLINE_SESSION(Error, TEXT("Pico platform service not available. Skipping FindSessions."));
        TArray<FBlueprintSessionResult> Results;
        OnFailure.Broadcast(Results);
    }
}

void UPicoFindSessionsCallbackProxy::OnCompleted(bool bSuccess)
{
    auto PicoSessionInterface = Online::GetSessionInterface(PICO_SUBSYSTEM);

    if (PicoSessionInterface.IsValid())
    {
        PicoSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(DelegateHandle);
    }

    TArray<FBlueprintSessionResult> Results;

    if (bSuccess && SearchObject.IsValid())
    {
        for (auto& Result : SearchObject->SearchResults)
        {
            FBlueprintSessionResult BPResult;
            BPResult.OnlineResult = Result;
            Results.Add(BPResult);
        }

        OnSuccess.Broadcast(Results);
    }
    else
    {
        OnFailure.Broadcast(Results);
    }
}