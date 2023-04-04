// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PICOXREditorSettings.generated.h"

/**
 * 
 */
UCLASS(config = Editor, defaultconfig)
class PICOXREDITOR_API UPICOXREditorSettings : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	/** Whether or not to show the PICOXR Input settings toolbar button */
	UPROPERTY(config, EditAnywhere, Category = "PICOXR Editor Settings")
	bool bShowPICOXRInputToolbarButton;
};
