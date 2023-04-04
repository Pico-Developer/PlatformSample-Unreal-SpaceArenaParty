// Fill out your copyright notice in the Description page of Project Settings.


#include "PICOGameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "PicoPresenceInterface.h"
#include "PICOPlatformDefines.h"
#include "JsonManager.h"

UPICOGameInstanceSubsystem::UPICOGameInstanceSubsystem()
    : UGameInstanceSubsystem()
{

}

void UPICOGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    JsonManager = NewObject<UJsonManager>();
    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        FOnlineSubsystemPico* PicoSubsystem = static_cast<FOnlineSubsystemPico*>(Subsystem);
        if (PicoSubsystem && PicoSubsystem->Init())
        {
            PicoPresenceInterface = PicoSubsystem->GetPicoPresenceInterface();
            if (PicoPresenceInterface)
            {
                PicoPresenceInterface->JoinIntentReceivedCallback.AddUObject(this, &UPICOGameInstanceSubsystem::OnJoinIntentChanged);
            }
        }
    }
}

void UPICOGameInstanceSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UPICOGameInstanceSubsystem::OnJoinIntentChanged(const FString& DeeplinkMessage, const FString& DestinationApiName, const FString& LobbySessionId, const FString& MatchSessionId, const FString& Extra)
{
    UE_LOG(LogPICO, Log, TEXT("%s, DeeplinkMessage: %s, DestinationApiName: %s, LobbySessionId: %s, MatchSessionId: %s, Extra: %s.!"), *PICO_FUNC_LINE, *DeeplinkMessage, *DestinationApiName, *LobbySessionId, *MatchSessionId, *Extra);
    bReceiveLaunchIntentChange = true;
    SaveDestinationApiName = DestinationApiName;
    SaveMatchSessionID = MatchSessionId;
}


bool UPICOGameInstanceSubsystem::GetIntentChanged(FString& OutDestinationApiName, FString& OutMatchSessionId)
{
    if (bReceiveLaunchIntentChange)
    {
        OutDestinationApiName = SaveDestinationApiName;
        OutMatchSessionId = SaveMatchSessionID;
        bReceiveLaunchIntentChange = false;
        SaveDestinationApiName = FString();
        SaveMatchSessionID = FString();
        return true;
    }
    return false;
}

void UPICOGameInstanceSubsystem::SetLogString(const FString& InNewLog)
{
    LogInfoCount++;
    if (LogInfoCount >= 40)
    {
        LogInfoCount = 0;
        TotalLog = FString();
    }
    TotalLog = InNewLog + TEXT("/n") + TotalLog;
    OnGameInstanceSubsystemLogChangedNotify.Broadcast(LogInfoCount);
}
