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

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PPF_Platform.h"
#include "Pico_DataStore.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DataStore, Log, All);
/**
 * 
 */

 /// @brief UPico_DataStore class.
UCLASS(BlueprintType)
class ONLINESUBSYSTEMPICO_API UPico_DataStore : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TMap<FString, FString> DataStoreMap;

	int32 NumKey;

	TArray<FString> KeyArray;

public:
	void InitParams(ppfDataStore* InppfDataStoreHandle);

	/** @brief With UserId as the key, find its index in the DataStoreMap data structure.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Misc|Data Store")
	int32 Contains(FString Key);

	/** @brief Find the user ID in the corresponding KeyArray through the index.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Misc|Data Store")
	FString GetKey(int32 Index);

	/** @brief Get the size of DataStoreMap .*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Misc|Data Store")
	int32 GetNumKeys();

	/** @brief With UserId as the key, find its value in the DataStoreMap data structure.*/
	UFUNCTION(BlueprintPure, Category = "Pico Platform|Misc|Data Store")
	FString GetValue(FString Key);
};
