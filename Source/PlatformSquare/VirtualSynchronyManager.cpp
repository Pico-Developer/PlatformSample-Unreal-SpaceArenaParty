// Fill out your copyright notice in the Description page of Project Settings.


#include "VirtualSynchronyManager.h"
#include "Core/Public/Serialization/BufferArchive.h"
#include "Pico_Networking.h"
#include "PICOPlatformDefines.h"

void UVirtualSynchronyManager::SaveLoadData(FArchive& Ar, FSyncInfo& SyncInfo)
{
    Ar << SyncInfo.UserLocation;

    Ar << SyncInfo.UserRotation;

    Ar << SyncInfo.UserDisplayName;

    Ar << SyncInfo.bIsMale;

    Ar << SyncInfo.MontageIndex;

    Ar << SyncInfo.PawnColor;
}

bool UVirtualSynchronyManager::ReadSyncInfo(FString& SendUserID, FSyncInfo& InSyncInfo)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    TArray<uint8> BinaryArray;
    bool ReadResult = UOnlinePicoNetworkingFunction::ReadPacket(this, BinaryArray, SendUserID);
    if (!ReadResult || BinaryArray.Num() == 0)
    {
        return false;
    }
    FMemoryReader FromBinary = FMemoryReader(BinaryArray, true);
    FromBinary.Seek(0);
    SaveLoadData(FromBinary, InSyncInfo);
    FromBinary.FlushCache();
    BinaryArray.Empty();
    FromBinary.Close();
    return true;
}

bool UVirtualSynchronyManager::SendSyncInfo(FSyncInfo& InSyncInfo)
{
    UE_LOG(LogPICO, Log, TEXT("%s"), *PICO_FUNC_LINE);
    FBufferArchive ToBinary;
    SaveLoadData(ToBinary, InSyncInfo);
    if (ToBinary.Num() <=0)
    {
        return false;
    }
    bool SendResult = UOnlinePicoNetworkingFunction::SendPacketToCurrentRoom(this, ToBinary, false);
    if (!SendResult)
    {
        UE_LOG(LogPICO, Log, TEXT("%s: Send sync info failed"), *PICO_FUNC_LINE);
    }
    ToBinary.FlushCache();
    ToBinary.Empty();
    ToBinary.Close();
    return SendResult;
}
