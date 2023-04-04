// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformGameSubsystem.h"
#include "PICOPlatformDefines.h"

DEFINE_LOG_CATEGORY(LogPICO);

UPlatformGameSubsystem::UPlatformGameSubsystem()
    : UWorldSubsystem()
{
}

void UPlatformGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogPICO, Log, TEXT("%s: PlatformGame subsystem initialized"), *PICO_FUNC_LINE);
}

void UPlatformGameSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

UVirtualSynchronyManager* UPlatformGameSubsystem::ConstructSyncManager()
{
    return NewObject<UVirtualSynchronyManager>(this);
}
