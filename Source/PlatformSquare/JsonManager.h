// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLaunchExtraInfo
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "LaunchExtra")
    FString SessionRoomID;

    UPROPERTY(BlueprintReadWrite, Category = "LaunchExtra")
    FString MapName;

    UPROPERTY(BlueprintReadWrite, Category = "LaunchExtra")
    FString RTCRoomID;

    UPROPERTY(BlueprintReadWrite, Category = "LaunchExtra")
    FString LocationInfo;
};

UCLASS()
class PLATFORMSQUARE_API UJsonManager : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
	FString MakeJsonLaunchExtra(FLaunchExtraInfo InLaunchExtraInfo);

    UFUNCTION(BlueprintCallable)
    bool ParseJsonLaunchExtra(const FString& InJsonStr, FLaunchExtraInfo& OutLaunchExtraInfo);
	
};
