// Fill out your copyright notice in the Description page of Project Settings.


#include "PicoSquarePlayerState.h"
#include "Pico_Room.h"
#include "Kismet/GameplayStatics.h"
#include "PlatformSquareGameModeBase.h"
#include "PICOGameInstanceSubsystem.h"



APicoSquarePlayerState::APicoSquarePlayerState()
    : APlayerState()
{
}

void APicoSquarePlayerState::BeginPlay()
{
    Super::BeginPlay();
    PicoGISubsystem = GetGameInstance()->GetSubsystem<UPICOGameInstanceSubsystem>();
    check(PicoGISubsystem);
}

void APicoSquarePlayerState::Login()
{
    PlatformLog(FString(TEXT("PicoSquarePlayerState::Login!")));
    LoginDelegate.Clear();
    LoginDelegate.BindDynamic(this, &APicoSquarePlayerState::OnLoginComplete);
    UOnlinePicoUserFunction::GetLoginUser(this, LoginDelegate);
}

void APicoSquarePlayerState::OnLoginComplete(bool bIsError, const FString& ErrorMessage, UPico_User* User)
{
    LoginDelegate.Clear();
    if (bIsError)
    {   
        PlatformLog(FString((TEXT("OnLoginComplete return Failed ErrorMessage : %s"), *ErrorMessage)));
        return;
    }
    PlatformLog(FString((TEXT("OnLoginComplete return successed! User Displayname : %s"), *User->GetDisplayName())));
    CurrentUser = User;
    bLoginComplete = true;
    APlatformSquareGameModeBase* GM = Cast<APlatformSquareGameModeBase>(UGameplayStatics::GetGameMode(this));
    if (GM)
    {
        GM->SetPlayerID(CurrentUser->GetID());
    }
    CheckUserDataAlready();

}

void APicoSquarePlayerState::GetLoginedUserFriendsAndRooms()
{
    PlatformLog(FString(TEXT("Call PicoSquarePlayerState::GetLoginedUserFriendsAndRooms!")));
    UserAndRoomList.Empty();
    LoginedInUserFriendsAndRoomsDelegate.Clear();
    LoginedInUserFriendsAndRoomsDelegate.BindDynamic(this, &APicoSquarePlayerState::OnGetUserFriendsAndRoomsComplete);
    UOnlinePicoUserFunction::PicoGetLoggedInUserAndRoom(this, LoginedInUserFriendsAndRoomsDelegate);
}

void APicoSquarePlayerState::OnGetUserFriendsAndRoomsComplete(bool bIsError, const FString& ErrorMessage, UPico_UserAndRoomArray* UserAndRoomArray)
{
    LoginedInUserFriendsAndRoomsDelegate.Clear();
    NextUserAndRoomArrayPage.Clear();
    if (bIsError)
    {
        PlatformLog(FString((TEXT("OnGetUserFriendsAndRoomsComplete return failed : %s"), *ErrorMessage)));
        return;
    }
    PlatformLog(FString(TEXT("OnGetUserFriendsAndRoomsComplete return successed!")));
    for (int32 i = 0; i < UserAndRoomArray->GetSize(); i++)
    {
        UserAndRoomList.Add(UserAndRoomArray->GetElement(i));
    }
    if (UserAndRoomArray->HasNextPage())
    {
        NextUserAndRoomArrayPage.BindDynamic(this, &APicoSquarePlayerState::OnGetUserFriendsAndRoomsComplete);
        UOnlinePicoUserFunction::PicoGetNextUserAndRoomArrayPage(this, UserAndRoomArray, NextUserAndRoomArrayPage);
    }
    else
    {
        bReadFriendsAndRoomsComplete = true;
        CheckUserDataAlready();
    }
}

void APicoSquarePlayerState::RefreshUserData()
{
    bBaseDataAlready = false;
    bLoginComplete = false;
    bReadFriendsAndRoomsComplete = false;
    Login();
    GetLoginedUserFriendsAndRooms();

    PicoDataReadyDelegate.Unbind();
    PicoDataReadyDelegate.BindUObject(this, &APicoSquarePlayerState::CallUserDataAlready);

}


void APicoSquarePlayerState::CheckUserDataAlready()
{
    if (bLoginComplete && bReadFriendsAndRoomsComplete)
    {
        bBaseDataAlready = true;
        PicoDataReadyDelegate.ExecuteIfBound();
        PicoDataReadyDelegate.Unbind();
    }
}

void APicoSquarePlayerState::CallUserDataAlready()
{
    NotifyDataAlready();
}

void APicoSquarePlayerState::PlatformLog(const FString& NewLog)
{
    if (PicoGISubsystem)
    {
        PicoGISubsystem->SetLogString(NewLog);
    }
}

