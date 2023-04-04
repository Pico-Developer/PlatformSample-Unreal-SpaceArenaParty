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

#include "PicoUpdateSessionCallbackProxy.h"
#include "OnlineSubsystemPicoPrivate.h"
#if ENGINE_MAJOR_VERSION > 4
#include "Online/CoreOnline.h"
#elif ENGINE_MINOR_VERSION > 24
#include "UObject/CoreOnline.h"
#endif
#include "Online.h"
#include "OnlineSessionInterfacePico.h"

UPicoUpdateSessionCallbackProxy::UPicoUpdateSessionCallbackProxy(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , UpdateCompleteDelegate(FOnUpdateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnUpdateCompleted))
    , bShouldEnqueueInMatchmakingPool(false)
{
}

UPicoUpdateSessionCallbackProxy* UPicoUpdateSessionCallbackProxy::SetSessionEnqueue(bool bShouldEnqueueInMatchmakingPool)
{
    UPicoUpdateSessionCallbackProxy* Proxy = NewObject<UPicoUpdateSessionCallbackProxy>();
    Proxy->SetFlags(RF_StrongRefOnFrame);
    Proxy->bShouldEnqueueInMatchmakingPool = bShouldEnqueueInMatchmakingPool;
    return Proxy;
}

void UPicoUpdateSessionCallbackProxy::Activate()
{
    auto PicoSessionInterface = Online::GetSessionInterface(PICO_SUBSYSTEM);

    if (PicoSessionInterface.IsValid())
    {
        UpdateCompleteDelegateHandle = PicoSessionInterface->AddOnUpdateSessionCompleteDelegate_Handle(UpdateCompleteDelegate);

        FOnlineSessionSettings Settings;
        Settings.bShouldAdvertise = bShouldEnqueueInMatchmakingPool;
#if ENGINE_MAJOR_VERSION > 4
        PicoSessionInterface->UpdateSession(NAME_GameSession, Settings);
#elif ENGINE_MINOR_VERSION > 24
        PicoSessionInterface->UpdateSession(GameSessionName, Settings);
#endif

    }
    else
    {
        UE_LOG_ONLINE_SESSION(Error, TEXT("Pico platform service not available. Skipping UpdateSession."));
        OnFailure.Broadcast();
    }
}

void UPicoUpdateSessionCallbackProxy::OnUpdateCompleted(FName SessionName, bool bWasSuccessful)
{
    auto PicoSessionInterface = Online::GetSessionInterface(PICO_SUBSYSTEM);

    if (PicoSessionInterface.IsValid())
    {
        PicoSessionInterface->ClearOnUpdateSessionCompleteDelegate_Handle(UpdateCompleteDelegateHandle);
    }

    if (bWasSuccessful)
    {
        OnSuccess.Broadcast();
    }
    else
    {
        OnFailure.Broadcast();
    }

}
