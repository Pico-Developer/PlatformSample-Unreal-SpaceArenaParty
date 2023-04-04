// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Pico_User.h"
#include "PicoSquarePlayerState.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FParsePicoUserArrayDelegate)

DECLARE_DELEGATE(FPicoDataReadyDelegate)

UCLASS()
class PLATFORMSQUARE_API APicoSquarePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APicoSquarePlayerState();
	virtual void BeginPlay() override;

protected:
	TSharedPtr<FPicoUserInterface> PicoUserInterface;

private:

	class UPICOGameInstanceSubsystem* PicoGISubsystem;
	

	FPicoDataReadyDelegate PicoDataReadyDelegate;

	// Current PICO user info class
	UPROPERTY()
	UPico_User* CurrentUser;

	// Friends and rooms array
	UPROPERTY()
	TArray<UPico_UserAndRoom*> UserAndRoomList;



	FGetLoginUser LoginDelegate;
	FGetLoggedInUserFriendsAndRooms LoginedInUserFriendsAndRoomsDelegate;
	FGetNextUserAndRoomArrayPage NextUserAndRoomArrayPage;

	bool bReadFriendsAndRoomsComplete;
	bool bLoginComplete;
	bool bBaseDataAlready;

public:
	UPROPERTY(BlueprintReadWrite)
	FLinearColor SelfColor;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPico_User* GetCurrentUser() { return CurrentUser; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UPico_UserAndRoom*> GetUserAndRommArray() {return UserAndRoomList; }

	// Get user info
	void Login();

	UFUNCTION()
	void OnLoginComplete(bool bIsError, const FString& ErrorMessage, UPico_User* User);

	// Get user and room info
	void GetLoginedUserFriendsAndRooms();

	UFUNCTION()
	void OnGetUserFriendsAndRoomsComplete(bool bIsError, const FString& ErrorMessage, UPico_UserAndRoomArray* UserAndRoomArray);



	UFUNCTION(BlueprintCallable)
	void RefreshUserData();


	//void ParseUserArrayObj(UPico_UserArray* InputUserArrayObj);

	void CheckUserDataAlready();

	void CallUserDataAlready();

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyDataAlready();

	void PlatformLog(const FString& NewLog);
	
};
