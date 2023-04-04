/*******************************************************************************
Copyright © 2015-2022 PICO Technology Co., Ltd.All rights reserved.

NOTICE：All information contained herein is, and remains the property of
PICO Technology Co., Ltd. The intellectual and technical concepts
contained herein are proprietary to PICO Technology Co., Ltd. and may be
covered by patents, patents in process, and are protected by trade secret or
copyright law. Dissemination of this information or reproduction of this
material is strictly forbidden unless prior written permission is obtained from
PICO Technology Co., Ltd.
*******************************************************************************/
// This plugin incorporates portions of the Unreal® Engine. Unreal® is a trademark or registered trademark of Epic Games, Inc.In the United States of America and elsewhere.
// Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc.All rights reserved.

#include "Pico_DataStore.h"
#include "OnlineSubsystemPicoNames.h"


DEFINE_LOG_CATEGORY(DataStore);
void UPico_DataStore::InitParams(ppfDataStore* InppfDataStoreHandle)
{
    UE_LOG(DataStore, Log, TEXT("Data store init!"));
    NumKey = ppf_DataStore_GetNumKeys(InppfDataStoreHandle);
    UE_LOG(DataStore, Log, TEXT("DataStore init GetNumKeys:%i"), NumKey);
    for (int i = 0; i < NumKey; i++)
    {
        FString Key = UTF8_TO_TCHAR(ppf_DataStore_GetKey(InppfDataStoreHandle, i));
        UE_LOG(DataStore, Log, TEXT("UPico_DataStore::InitParams Key:%s"), *Key);
        KeyArray.Add(Key);
        FString Value = UTF8_TO_TCHAR(ppf_DataStore_GetValue(InppfDataStoreHandle, TCHAR_TO_UTF8(*Key)));
        DataStoreMap.Add(Key, Value);
    }
}

int32 UPico_DataStore::Contains(FString Key)
{
    return KeyArray.Find(Key);
}

FString UPico_DataStore::GetKey(int32 Index)
{
    if (KeyArray.IsValidIndex(Index))
    {
        return KeyArray[Index];
    }
    UE_LOG(DataStore, Log, TEXT("GetKey : Not vaild index %i"), Index);
    return FString();
}

int32 UPico_DataStore::GetNumKeys()
{
    return NumKey;
}

FString UPico_DataStore::GetValue(FString Key)
{
    if (DataStoreMap.Contains(Key))
    {
        return *DataStoreMap.Find(Key);
    }
    UE_LOG(DataStore, Log, TEXT("GetKey : Not vaild key: %s"), *Key);
    return FString();
}
