// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PICOGameInstanceSubsystem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FGameInstanceSubsystemLogChangedNotify, UPICOGameInstanceSubsystem, OnGameInstanceSubsystemLogChangedNotify, int, LogCount);

UCLASS()
class PLATFORMSQUARE_API UPICOGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    UPICOGameInstanceSubsystem();

    // Begin USubsytem
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // End

protected:
	TSharedPtr<class FPicoPresenceInterface> PicoPresenceInterface;

	UPROPERTY()
	FString TotalLog;

	int32 LogInfoCount;    

    bool bReceiveLaunchIntentChange;
    FString SaveMatchSessionID;
    FString SaveDestinationApiName;

public:
	UPROPERTY(BlueprintReadOnly)
	class UJsonManager* JsonManager;

    UPROPERTY(BlueprintAssignable)
    FGameInstanceSubsystemLogChangedNotify OnGameInstanceSubsystemLogChangedNotify;

	void OnJoinIntentChanged(const FString& DeeplinkMessage, const FString& DestinationApiName, const FString& LobbySessionId, const FString& MatchSessionId, const FString& Extra);

	UFUNCTION(BlueprintCallable)
	bool GetIntentChanged(FString& OutDestinationApiName, FString& OutMatchSessionId);

	UFUNCTION(BlueprintCallable)
	void SetLogString(const FString& InNewLog);

	UFUNCTION(BlueprintCallable)
	FString GetTotalLog(){return TotalLog;}
	
};
