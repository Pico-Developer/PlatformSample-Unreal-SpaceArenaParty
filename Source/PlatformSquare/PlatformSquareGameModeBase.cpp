// Copyright Epic Games, Inc. All Rights Reserved.


#include "PlatformSquareGameModeBase.h"
#include "Core/Public/Serialization/BufferArchive.h"
#include "Pico_Networking.h"
#include "Kismet/GameplayStatics.h"
#include "PicoSquarePlayerState.h"
#include "Online.h"
#include "OnlineSubsystemPico.h"
#include "OnlineSubsystemUtils.h"
#include "PlatformGameSubsystem.h"
#include "VirtualSynchronyManager.h"
#include "PICOGameInstanceSubsystem.h"
#include "PICOPlatformDefines.h"

FGMGetRoomListCompleteDelegate APlatformSquareGameModeBase::GetRoomListDelegate;

TArray<FVector> PlayerSpawnLocations = { FVector(-60.f, 1260.f, 98.f), FVector(-1280.0f, -80.0f, 98.f), FVector(-680.0f, -1260.0f, 98.f), FVector(730.0f, -1040.0f, 98.f), FVector(1080.0f, 510.0f, 98.f) };

APlatformSquareGameModeBase::APlatformSquareGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/BP_RoomCharacter"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        RoomPlayerClass = PlayerPawnBPClass.Class;
    }
    if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
    {
        FOnlineSubsystemPico* PicoSubsystem = static_cast<FOnlineSubsystemPico*>(Subsystem);
        if (PicoSubsystem && PicoSubsystem->Init())
        {
            PicoRoomInterface = PicoSubsystem->GetPicoRoomInterface();
            PicoPresenceInterface = PicoSubsystem->GetPicoPresenceInterface();
            if (PicoRoomInterface)
            {
                PicoRoomInterface->RoomUpdateNotify.AddUObject(this, &APlatformSquareGameModeBase::OnPicoRoomUpdateNotification);
                PicoRoomInterface->RoomJoinNotify.AddUObject(this, &APlatformSquareGameModeBase::OnPicoRoomJoinNotification);
                PicoRoomInterface->RoomLeaveNotify.AddUObject(this, &APlatformSquareGameModeBase::OnPicoRoomLeaveNotification);
            }
        }
    }
}


void APlatformSquareGameModeBase::BeginPlay()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    Super::BeginPlay();
    PicoGISubsystem = GetGameInstance()->GetSubsystem<UPICOGameInstanceSubsystem>();
    check(PicoGISubsystem);
    GetWorld()->GetTimerManager().SetTimer(SendPackTimerHandle, this, &APlatformSquareGameModeBase::SendUserPacket, 2.0f, true);
    GetWorld()->GetTimerManager().SetTimer(ReadPackTimerHandle, this, &APlatformSquareGameModeBase::CollectPlayerInfo, 0.05f, true);
    if (PicoPresenceInterface && PicoRoomInterface)
    {
        PlatformLog(FString(TEXT("PicoSubsystem init successed!")));
    }
    else
    {
        PlatformLog(FString(TEXT("PicoSubsystem init failed!")));
    }
    UPlatformGameSubsystem* PGSubsystem = GetWorld()->GetSubsystem<UPlatformGameSubsystem>();
    if (PGSubsystem)
    {
        SyncManager = PGSubsystem->ConstructSyncManager();
        check(SyncManager);
    }
}

void APlatformSquareGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void APlatformSquareGameModeBase::OnPicoRoomUpdateNotification(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("OnPicoRoomUpdateNotification!")));
    if (!bIsError)
    {
        if (!CurrentPicoRoom)
        {
            return;
        }

        CurrentPicoRoom = Room;
        SpawnRoomUser(bIsError, ErrorMessage, nullptr);
    }
}

void APlatformSquareGameModeBase::OnPicoRoomJoinNotification(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (!bIsError)
    {
        if (!CurrentPicoRoom)
        {
            return;
        }
        PlatformLog(FString(TEXT("OnPicoRoomJoinNotification!")));
        CurrentPicoRoom = Room;
        SpawnRoomUser(bIsError, ErrorMessage, nullptr);
    }
}

void APlatformSquareGameModeBase::OnPicoRoomLeaveNotification(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (!bIsError)
    {
        if (!CurrentPicoRoom)
        {
            return;
        }
        PlatformLog(FString(TEXT("OnPicoRoomLeaveNotification!")));
        CurrentPicoRoom = Room;
        SpawnRoomUser(bIsError, ErrorMessage, nullptr);
    }
}


void APlatformSquareGameModeBase::CollectPlayerInfo()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (CurrentPicoRoom)
    {
        FString UserID;
        FSyncInfo ReceiveInfo;
        bool ReadRes = SyncManager->ReadSyncInfo(UserID, ReceiveInfo);
        if (UserID == PlayerId)
        {
            return;
        }
        if (!ReadRes)
        {
            return;
        }
        ARoomPlayerCharacter* TargetCharacter = *CurrentPlayers.Find(UserID);
        if (TargetCharacter)
        {
            TargetCharacter->CharacterMoveTo(ReceiveInfo.UserLocation);
            TargetCharacter->SetPlayerColor(ReceiveInfo.PawnColor);

        }
        else
        {
            UE_LOG(LogPICO, Log, TEXT("%s, can't find valid character in CurrentPlayers"), *PICO_LINE);
        }
    }

}

void APlatformSquareGameModeBase::CreateRoom(ERoomJoinPolicy InJoinPolicy, FLinearColor InColor)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("PlatformSquareGameModeBase::CreateRoom!")));
    FPicoRoomOptions RoomOptions;
    RoomColor = InColor;
    RoomOptions.RoomId = TEXT("ThisIsXX");
    RoomOptions.MaxUserResults = 20;
    RoomOptions.bExcludeRecentlyMet = false;
    RoomOptions.DataStoreKey = TEXT("");
    RoomOptions.DataStoreValue = TEXT("");
    RoomOptions.TimeWindow = ETimeWindow::OneDay;
    RoomOptions.bTurnOffUpdates = false;
    RoomOptions.Ordering = EUserOrdering::PresenceAlphabetical;
    RoomCreateJoinDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnCreateRoomComplete);
    PicoRoomInterface->CreateAndJoinPrivate2(InJoinPolicy, 20, RoomOptions, RoomCreateJoinDelegate);
}

void APlatformSquareGameModeBase::OnCreateRoomComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (bIsError)
    {
        PlatformLog(FString((TEXT("CreateRoomComplete return Failed! room info : %s"), *ErrorMessage)));
        return;
    }
    PlatformLog(FString((TEXT("CreateRoomComplete return successed! room info : %s"), *Room->GetRoomID())));
    CurrentPicoRoom = Room;
    FString RoomString = RoomColor.ToString();
    SetRoomDescription(RoomString, Room->GetRoomID());
    SpawnRoomUser(bIsError, ErrorMessage, nullptr);
}


void APlatformSquareGameModeBase::UpdateRoomOption(ERoomJoinPolicy NewJoinPolicy)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("Call UpdateRoomOption")));
    if (CurrentPicoRoom && PicoRoomInterface)
    {
        RoomUpdatePrivateRoomJoinPolicyDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnUpdateRoomOptionComplete);
        PicoRoomInterface->UpdatePrivateRoomJoinPolicy(CurrentPicoRoom->GetRoomID(), NewJoinPolicy, RoomUpdatePrivateRoomJoinPolicyDelegate);
    }
}

void APlatformSquareGameModeBase::OnUpdateRoomOptionComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    RoomUpdatePrivateRoomJoinPolicyDelegate.Clear();
    if (bIsError)
    {
        PlatformLog(FString((TEXT("OnUpdateRoomOptionComplete return Failed! room info : %s"), *ErrorMessage)));
        return;
    }
    PlatformLog(FString((TEXT("OnUpdateRoomOptionComplete return successed! room info : %s"), *Room->GetRoomID())));
    CurrentPicoRoom = Room;
    SpawnRoomUser(bIsError, ErrorMessage, nullptr);
}


void APlatformSquareGameModeBase::SetRoomDescription(FString InDescription, FString RoomID)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("Call SetRoomDescription")));
    RoomSetDescriptionDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnSetRoomDescriptionComplete);
    PicoRoomInterface->SetDescription(RoomID, InDescription, RoomSetDescriptionDelegate);
}

void APlatformSquareGameModeBase::OnSetRoomDescriptionComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    RoomSetDescriptionDelegate.Clear();
    if (bIsError)
    {
        PlatformLog(FString((TEXT("OnSetRoomDescriptionComplete return failed room info : %s"), *ErrorMessage)));
        return;
    }
    PlatformLog(FString((TEXT("OnSetRoomDescriptionComplete return successed room Description: %s"), *Room->GetDescription())));
    CurrentPicoRoom = Room;
    OnGameModeJoinOrLeaveNotify.Broadcast(true, Room);
}

void APlatformSquareGameModeBase::SendUserPacket()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (CurrentPicoRoom)
    {
        FSyncInfo SyncInfo;
        APawn* CurrentPawn = UGameplayStatics::GetPlayerPawn(this, 0);
        if (!CurrentPawn)
        {   
            return;
        }
        APicoSquarePlayerState* PlayerState = Cast<APicoSquarePlayerState>(CurrentPawn->GetPlayerState());
        if (!PlayerState)
        {
            return;
        }
        if (!PlayerState->GetCurrentUser())
        {
            PlatformLog(FString(TEXT("Send Packet failed user not login!")));
            return;
        }
        FString UserDisplayName = PlayerState->GetCurrentUser()->GetDisplayName();
        FVector PawnLocation = CurrentPawn->GetActorLocation();
        FRotator PawnRotation = CurrentPawn->GetActorRotation();
        bool bIsMale = PlayerState->GetCurrentUser()->GetGender() == EUserGender::Male;
        SyncInfo.bIsMale = bIsMale;
        SyncInfo.MontageIndex = 2;
        SyncInfo.UserDisplayName = UserDisplayName;
        SyncInfo.UserLocation = PawnLocation;
        SyncInfo.UserRotation = PawnRotation;
        SyncInfo.PawnColor = PlayerState->SelfColor;
        bool Res = SyncManager->SendSyncInfo(SyncInfo);
        if (!Res)
        {
            UE_LOG(LogPICO, Log, TEXT("%s, SendInfo failed!"), *PICO_FUNC_LINE);
        }
    }
}

// ModerateRoom list and friend's room list
void APlatformSquareGameModeBase::ReadRoomList(FGMGetRoomListCompleteDelegate InGetListDelegate)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("PlatformSquareGameModeBase::ReadRoomList!")));
    RoomList.Empty();
    RoomGetModerateRoomsDelegate.Clear();
    RoomIndex = 0;
    RoomGetModerateRoomsDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnGetRoomListComplete);
    APlatformSquareGameModeBase::GetRoomListDelegate = InGetListDelegate;
    PicoRoomInterface->GetModeratedRooms(RoomIndex, 20, RoomGetModerateRoomsDelegate);
    RoomIndex++;
}

void APlatformSquareGameModeBase::OnGetRoomListComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_RoomArray* RoomArray)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    RoomGetModerateRoomsDelegate.Clear();
    if (bIsError)
    {
        PlatformLog(FString((TEXT("OnGetRoomListComplete return Failed ErrorMessage : %s"), *ErrorMessage)));
        if (APlatformSquareGameModeBase::GetRoomListDelegate.IsBound())
        {
            APlatformSquareGameModeBase::GetRoomListDelegate.Execute(false, ErrorMessage);
            APlatformSquareGameModeBase::GetRoomListDelegate.Unbind();
        }
        return;
    }
    PlatformLog(FString(TEXT("OnGetRoomListComplete return successed")));
    int32 RoomArraySize = RoomArray->GetSize();
    for (int32 i = 0; i < RoomArraySize; i++)
    {
        RoomList.Add(RoomArray->GetElement(i));
    }
    if (RoomArraySize == 20)
    {
        RoomGetModerateRoomsDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnGetRoomListComplete);
        PicoRoomInterface->GetModeratedRooms(RoomIndex, 20, RoomGetModerateRoomsDelegate);
        RoomIndex++;
    }
    else
    {
        GetLoggedInUserFriendsAndRoomsDelegate.Clear();
        GetLoggedInUserFriendsAndRoomsDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnGetUserFriendsAndRoomsComplete);
        UOnlinePicoUserFunction::PicoGetLoggedInUserAndRoom(this, GetLoggedInUserFriendsAndRoomsDelegate);
    }
}

void APlatformSquareGameModeBase::OnGetUserFriendsAndRoomsComplete(bool bIsError, const FString& ErrorMessage, UPico_UserAndRoomArray* UserAndRoomArray)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    GetLoggedInUserFriendsAndRoomsDelegate.Clear();
    if (bIsError)
    {
        PlatformLog(FString((TEXT("OnGetUserFriendsAndRoomsComplete return failed ErrorMessage : %s"), *ErrorMessage)));
        if (APlatformSquareGameModeBase::GetRoomListDelegate.IsBound())
        {
            APlatformSquareGameModeBase::GetRoomListDelegate.Execute(false, ErrorMessage);
            APlatformSquareGameModeBase::GetRoomListDelegate.Unbind();
        }
        return;
    }
    PlatformLog(FString(TEXT("OnGetUserFriendsAndRoomsComplete return successed!")));
    int32 UserAndRoomArraySize = UserAndRoomArray->GetSize();
    TArray<UPico_Room*> UserRoomArray;
    // remove duplicate rooms
    for (int32 i = 0; i < UserAndRoomArraySize; i++)
    {
        UPico_Room* EachRoom = UserAndRoomArray->GetElement(i)->GetRoom();
        if (EachRoom->GetRoomID() == FString("0"))
        {
            continue;
        }
        bool bIsSameRoom = false;
        for (auto ModeratedRoom: RoomList)
        {
            if (ModeratedRoom->GetRoomID() == EachRoom->GetRoomID())
            {
                bIsSameRoom = true;
                break;
            }    
        }
        if (!bIsSameRoom)
        {
            UserRoomArray.Add(EachRoom);
        }
    }
    RoomList.Append(UserRoomArray);
    if (UserAndRoomArray->HasNextPage())
    {
        PlatformLog(FString(TEXT("UserAndRoomArray have next page!")));
        GetNextUserAndRoomArrayPageDelegate.Clear();
        GetNextUserAndRoomArrayPageDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnGetUserFriendsAndRoomsComplete);
        UOnlinePicoUserFunction::PicoGetNextUserAndRoomArrayPage(this, UserAndRoomArray, GetNextUserAndRoomArrayPageDelegate);
    }
    else
    {
        if (APlatformSquareGameModeBase::GetRoomListDelegate.IsBound())
        {
            APlatformSquareGameModeBase::GetRoomListDelegate.Execute(true, FString());
            APlatformSquareGameModeBase::GetRoomListDelegate.Unbind();
        }
    }
}

UPico_Room* APlatformSquareGameModeBase::GetCurrentRoom()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    return CurrentPicoRoom;
}

void APlatformSquareGameModeBase::SpawnRoomUser(bool bIsError, const FString& ErrorMessage, UPico_UserArray* UserArray)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (!CurrentPicoRoom || bIsError)
    {
        return;
    }
    
    // If the UserArray passed in by the function is empty, use the UserArray in the current room instead.
    if (!UserArray)
    {
        UserArray = CurrentPicoRoom->GetUsers();
        CurrentUserIdInRoom.Empty();
    }
    for (int i = 0; i < UserArray->GetSize(); i ++)
    {
        // Traverse the array and exclude one's own ID.
        UPico_User* UserInfo = UserArray->GetElement(i);
        FString EachId = UserInfo->GetID();
        CurrentUserIdInRoom.Add(EachId);
        if (EachId == PlayerId)
        {
            continue;
        }
        if (CurrentPlayers.Contains(UserInfo->GetID()))
        {
            continue;
        }
        int32 CurrentPlayerNum = CurrentPlayers.Num();

        ARoomPlayerCharacter* NewPlayer = GetWorld()->SpawnActor<ARoomPlayerCharacter>(RoomPlayerClass, /*FVector(-620.f, -920.f, 98.f)*/PlayerSpawnLocations[CurrentPlayerNum % 5], FRotator());
        if(NewPlayer)
        {
            NewPlayer->SetBaseInfo(EachId, UserInfo->GetDisplayName());
            CurrentPlayers.Add(EachId, NewPlayer);
        }
        else
        {
            PlatformLog(FString(TEXT("Spawn character failed!")));
        }

    }
    TArray<FString> MapPlayerIdArray;
    CurrentPlayers.GenerateKeyArray(MapPlayerIdArray);
    for (FString Each : MapPlayerIdArray)
    {
        if (!CurrentUserIdInRoom.Contains(Each))
        {
            ARoomPlayerCharacter* LeaveCharacter = *CurrentPlayers.Find(Each);
            if (LeaveCharacter)
            {
                PlatformLog(FString(TEXT("Destory character which leaved")));
                LeaveCharacter->Destroy();
                CurrentPlayers.Remove(Each);
            }

        }
    }
}

void APlatformSquareGameModeBase::InviteSelectUsers(bool bLaunch)
{   
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("Call APlatformSquareGameModeBase::InviteSelectUsers!")));
    if (bLaunch)
    {
        ClearInviteUsers();
    }
    SetCurrentPresence();
}

void APlatformSquareGameModeBase::OnPresenceSetComplete(bool bIsSuccessed, const FString& ErrorMessage)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PresenceSetDelegate.Unbind();
    if (bIsSuccessed)
    {
        PlatformLog(FString(TEXT("OnPresenceSetComplete return successed!")));
        if (InvitesUser.Num() == 0)
        {
            LaunchInvitePanelDelegate.Unbind();
            PlatformLog(FString(TEXT("Call LaunchInvitePanel!")));
            LaunchInvitePanelDelegate.BindUObject(this, &APlatformSquareGameModeBase::OnLaunchInvitePanelComplete);
            PicoPresenceInterface->LaunchInvitePanel(LaunchInvitePanelDelegate);
        }
        else
        {
            SentInvitesDelegate.Unbind();
            PlatformLog(FString(TEXT("Call PresenceSendInvites!")));
            SentInvitesDelegate.BindUObject(this, &APlatformSquareGameModeBase::OnPresenceSendInvitesComplete);
            PicoPresenceInterface->PresenceSendInvites(InvitesUser, SentInvitesDelegate);
        }
    }
    else
    {
        PlatformLog(FString((TEXT("OnPresenceSetComplete return failed : %s"), *ErrorMessage)));
    }
}

void APlatformSquareGameModeBase::OnPresenceSendInvitesComplete(bool bIsSuccessed, const FString& ErrorMessage)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    SentInvitesDelegate.Unbind();
    if (bIsSuccessed)
    {
        PlatformLog(FString(TEXT("OnPresenceSendInvitesComplete return successed!")));
    }
    else
    {
        PlatformLog(FString((TEXT("OnPresenceSendInvitesComplete return failed : %s"), *ErrorMessage)));
    }
}

void APlatformSquareGameModeBase::OnLaunchInvitePanelComplete(bool bIsSuccessed, const FString& ErrorMessage)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    LaunchInvitePanelDelegate.Unbind();
    if (bIsSuccessed)
    {
        PlatformLog(FString(TEXT("OnLaunchInvitePanelComplete return successed!")));
    }
    else
    {
        PlatformLog(FString((TEXT("OnLaunchInvitePanelComplete return failed : %s"), *ErrorMessage)));
    }
}

void APlatformSquareGameModeBase::AddInviteUsers(FString InUserID)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (!InUserID.IsEmpty())
    {
        InvitesUser.AddUnique(InUserID);
    }
}

void APlatformSquareGameModeBase::ClearInviteUsers()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    InvitesUser.Empty();
}

void APlatformSquareGameModeBase::SubInviteUsers(FString InUserID)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (!InUserID.IsEmpty() && InvitesUser.Contains(InUserID))
    {
        InvitesUser.Remove(InUserID);
    }
}

void APlatformSquareGameModeBase::SetCurrentPresence()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("Call PlatformSquareGameModeBase::SetCurrentPresence!")));
    if (!PicoPresenceInterface)
    {
        return;
    }
    FString MatchSessionID = FString();
    if (CurrentPicoRoom)
    {
        MatchSessionID = CurrentPicoRoom->GetRoomID();
    }
    PresenceSetDelegate.Unbind();
    PresenceSetDelegate.BindUObject(this, &APlatformSquareGameModeBase::OnPresenceSetComplete);
    PlatformLog(FString((TEXT("Call APlatformSquareGameModeBase::SetCurrentPresence MatchSessionID = %s!"), *MatchSessionID)));
    FString Destination = GetDestinationConfig();
    PicoPresenceInterface->PresenceSet(Destination, FString(), MatchSessionID, true, FString(), PresenceSetDelegate);
}

void APlatformSquareGameModeBase::JoinRoom(const FString& RoomID)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    RoomJoinDelegate.Clear();
    RoomJoinDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnJoinRoomComplete);
    PlatformLog(FString(TEXT("Call APlatformSquareGameModeBase::JoinRoom!")));
    PicoRoomInterface->Join2(RoomID, FPicoRoomOptions(), RoomJoinDelegate);
}

void APlatformSquareGameModeBase::OnJoinRoomComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (bIsError)
    {
        PlatformLog(FString((TEXT("OnJoinRoomComplete return Failed! ErrorMessage : %s"), *ErrorMessage)));
        return;
    }
    PlatformLog(FString((TEXT("OnJoinRoomComplete return successed! room info : %s"), *Room->GetRoomID())));
    CurrentPicoRoom = Room;
    OnGameModeJoinOrLeaveNotify.Broadcast(true, Room);
    SpawnRoomUser(bIsError, ErrorMessage, nullptr);
}

void APlatformSquareGameModeBase::LeaveRoom()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("Call PlatformSquareGameModeBase::LeaveRoom!")));
    if (CurrentPicoRoom)
    {
        RoomLeaveDelegate.Clear();
        RoomLeaveDelegate.BindDynamic(this, &APlatformSquareGameModeBase::OnLeaveRoomComplete);
        PicoRoomInterface->Leave(CurrentPicoRoom->GetRoomID(), RoomLeaveDelegate);
    }
}

void APlatformSquareGameModeBase::OnLeaveRoomComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (bIsError)
    {
        PlatformLog(FString((TEXT("OnLeaveRoomComplete return Failed! room info : %s"), *ErrorMessage)));
        return;
    }
    PlatformLog(FString((TEXT("OnLeaveRoomComplete return successed! room info : %s"), *Room->GetRoomID())));
    OnGameModeJoinOrLeaveNotify.Broadcast(false, nullptr);
    CurrentPicoRoom = nullptr;
    ClearPlayers();
}   

void APlatformSquareGameModeBase::ClearCurrentPresence()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PlatformLog(FString(TEXT("Call PlatformSquareGameModeBase::ClearCurrentPresence!")));
    PresenceClearDelegate.Unbind();
    PresenceClearDelegate.BindUObject(this, &APlatformSquareGameModeBase::OnPreseneClearComplete);
    PicoPresenceInterface->PresenceClear(PresenceClearDelegate);
}

void APlatformSquareGameModeBase::OnPreseneClearComplete(bool bIsSuccessed, const FString& ErrorMessage)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    PresenceClearDelegate.Unbind();
    if (bIsSuccessed)
    {
        PlatformLog(FString(TEXT("OnPreseneClearComplete return successed!")));
    }
    else
    {
        PlatformLog(FString((TEXT("OnPreseneClearComplete return failed : %s"), *ErrorMessage)));
    }
}

void APlatformSquareGameModeBase::ClearPlayers()
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    for (auto It = CurrentPlayers.CreateIterator(); It; ++It)
    {
        It->Value->Destroy();
    }
    CurrentPlayers.Empty();
}

void APlatformSquareGameModeBase::PlatformLog(const FString& NewLog)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    if (PicoGISubsystem)
    {
        PicoGISubsystem->SetLogString(NewLog);
    }
}

FString APlatformSquareGameModeBase::GetDestinationConfig()
{
    return GConfig->GetStr(TEXT("/Script/OnlineSubsystemPico.OnlinePicoSettings"), TEXT("Destination"), GEngineIni);
}

