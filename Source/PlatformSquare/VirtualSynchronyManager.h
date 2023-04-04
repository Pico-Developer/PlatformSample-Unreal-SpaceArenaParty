// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VirtualSynchronyManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, meta = (DisplayName = "SyncInfo"))
struct FSyncInfo
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite)
    FVector UserLocation; 

    UPROPERTY(BlueprintReadWrite)
    FRotator UserRotation;

    UPROPERTY(BlueprintReadWrite)
    FString UserDisplayName; 

    UPROPERTY(BlueprintReadWrite)
    bool bIsMale;

    UPROPERTY(BlueprintReadWrite)
    int32 MontageIndex;

    UPROPERTY(BlueprintReadWrite)
    FLinearColor PawnColor;
};


UCLASS(BlueprintType, Blueprintable)
class PLATFORMSQUARE_API UVirtualSynchronyManager : public UObject
{
	GENERATED_BODY()
	
public:
    // Convert binary data to and from structured data.
    void SaveLoadData(FArchive& Ar, FSyncInfo& SyncInfo);

    // Read synchronized data within a room.
    bool ReadSyncInfo(FString& SendUserID, FSyncInfo& InSyncInfo);

    // Send data to be synchronized within a room.
    bool SendSyncInfo(FSyncInfo& InSyncInfo);
};
