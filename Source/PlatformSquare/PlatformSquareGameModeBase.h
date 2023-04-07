// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Pico_Room.h"
#include "RoomPlayerCharacter.h"
#include "Pico_User.h"
#include "PicoPresenceInterface.h"
#include "Delegates/Delegate.h"
#include "PlatformSquareGameModeBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FGameModeJoinOrLeaveNotify, APlatformSquareGameModeBase, OnGameModeJoinOrLeaveNotify, bool, bIsJoin, UPico_Room*, Room);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FGMGetRoomListCompleteDelegate, bool, bIsSuccessed, const FString, ErrorMessage);

UCLASS()
class PLATFORMSQUARE_API APlatformSquareGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
    
    UPROPERTY()
    class UVirtualSynchronyManager* SyncManager;

    class UPICOGameInstanceSubsystem* PicoGISubsystem;

    TMap<FString, ARoomPlayerCharacter*> CurrentPlayers;

    TArray<FString> CurrentUserIdInRoom;

    TArray<UPico_Room*> RoomList;

    // Use in GetRoomList
    int32 RoomIndex;

    FString PlayerId;

    TArray<FString> InvitesUser;

    

    UPROPERTY()
    UPico_Room* CurrentPicoRoom;

    FTimerHandle SendPackTimerHandle;
    FTimerHandle ReadPackTimerHandle;
    FGetNextUserPage NextUserPageDelegate;
    FGetLoggedInUserFriendsAndRooms GetLoggedInUserFriendsAndRoomsDelegate;
    FGetNextUserAndRoomArrayPage GetNextUserAndRoomArrayPageDelegate;

public:
    UPROPERTY(BlueprintAssignable)
    FGameModeJoinOrLeaveNotify OnGameModeJoinOrLeaveNotify;

    // Join or create room' scene tone
    UPROPERTY(BlueprintReadWrite)
    FLinearColor RoomColor;

protected:

    TSubclassOf<APawn> RoomPlayerClass;
    TSharedPtr<FPicoRoomInterface> PicoRoomInterface;
    TSharedPtr<FPicoPresenceInterface> PicoPresenceInterface;

    static FGMGetRoomListCompleteDelegate GetRoomListDelegate;



private:
    void OnPicoRoomUpdateNotification(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);
    void OnPicoRoomJoinNotification(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);
    void OnPicoRoomLeaveNotification(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);

public:

    APlatformSquareGameModeBase();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    void SetPlayerID(const FString& InPlayID){PlayerId = InPlayID;}

    void CollectPlayerInfo();


    // CreatePrivateRoom
    FRoomCreateAndJoinPrivate2 RoomCreateJoinDelegate;

    UFUNCTION()
    void OnCreateRoomComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);

    UFUNCTION(BlueprintCallable)
    void CreateRoom(ERoomJoinPolicy InJoinPolicy, FLinearColor InColor);

    // UpdateRoomJoinPolicy
    FRoomUpdatePrivateRoomJoinPolicy RoomUpdatePrivateRoomJoinPolicyDelegate;

    UFUNCTION(BlueprintCallable)
    void UpdateRoomOption(ERoomJoinPolicy NewJoinPolicy);

    UFUNCTION()
    void OnUpdateRoomOptionComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);

    // SetRoomDescription
    FRoomSetDescription RoomSetDescriptionDelegate;

    void SetRoomDescription(FString InDescription, FString RoomID);

    UFUNCTION()
    void OnSetRoomDescriptionComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);

    // JoinRoom
    FRoomJoin2 RoomJoinDelegate;

    UFUNCTION(BlueprintCallable)
    void JoinRoom(const FString& RoomID);

    UFUNCTION()
    void OnJoinRoomComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);

    // LeaveRoom

    FRoomLeave RoomLeaveDelegate;

    UFUNCTION(BlueprintCallable)
    void LeaveRoom();

    UFUNCTION()
    void OnLeaveRoomComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_Room* Room);


    void SendUserPacket();

    // GetModeratedRooms
    FRoomGetModeratedRooms RoomGetModerateRoomsDelegate;

    UFUNCTION(BlueprintCallable)
    void ReadRoomList(FGMGetRoomListCompleteDelegate InGetListDelegate);

    UFUNCTION()
    void OnGetRoomListComplete(bool bIsError, int ErrorCode, const FString& ErrorMessage, UPico_RoomArray* RoomArray);

    UFUNCTION()
    void OnGetUserFriendsAndRoomsComplete(bool bIsError, const FString& ErrorMessage, UPico_UserAndRoomArray* UserAndRoomArray);

    UFUNCTION(BlueprintCallable)
    TArray<UPico_Room*> GetRoomList(){return RoomList;}

    UFUNCTION(BlueprintCallable)
    UPico_Room* GetCurrentRoom();


    UFUNCTION()
    void SpawnRoomUser(bool bIsError, const FString& ErrorMessage, UPico_UserArray* UserArray);

    UFUNCTION(BlueprintCallable)
    void InviteSelectUsers(bool bLaunch);



    FOnPresenceSetComplete PresenceSetDelegate;
    void OnPresenceSetComplete(bool bIsSuccessed, const FString& ErrorMessage);

    FOnSentInvitesComplete SentInvitesDelegate;
    void OnPresenceSendInvitesComplete(bool bIsSuccessed, const FString& ErrorMessage);

    FOnLaunchInvitePanelComplete LaunchInvitePanelDelegate;
    void OnLaunchInvitePanelComplete(bool bIsSuccessed, const FString& ErrorMessage);

    UFUNCTION(BlueprintCallable)
    void AddInviteUsers(FString InUserID);

    UFUNCTION(BlueprintCallable)
    void ClearInviteUsers();

    UFUNCTION(BlueprintCallable)
    void SubInviteUsers(FString InUserID);

    void SetCurrentPresence();
    

    // Clear Presence
    FOnPresenceClearComplete PresenceClearDelegate;

    UFUNCTION(BlueprintCallable)
    void ClearCurrentPresence();

    void OnPreseneClearComplete(bool bIsSuccessed, const FString& ErrorMessage);

    void ClearPlayers();


    void PlatformLog(const FString& NewLog);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    FString GetDestinationConfig();


};
