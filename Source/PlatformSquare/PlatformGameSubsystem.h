// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VirtualSynchronyManager.h"
#include "PlatformGameSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PLATFORMSQUARE_API UPlatformGameSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPlatformGameSubsystem();

	// Begin USubsytem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Construct Sync Manager"))
	UVirtualSynchronyManager* ConstructSyncManager();

};
