﻿//Unreal® Engine, Copyright 1998 – 2022, Epic Games, Inc. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PICOXRMRC/Private/PXR_MRCState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PXR_MRCFunctionLibrary.generated.h"

class UTextureRenderTarget2D;
struct FPXRTrackedCamera;
class UMaterialInstanceDynamic;

UCLASS()
class PICOXRMRC_API UPICOXRMRCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable,Category="PXR|PXRMRC")
	static bool IsMrcActivated();

	UFUNCTION(BlueprintCallable,Category= "PXR|PXRMRC")
	static bool GetInGameThirdCameraRT(UTextureRenderTarget2D* &Background_RT, UTextureRenderTarget2D*& Forgound_RT);

	//UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static void SimulateEnableMRC(bool enable);

	UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static void EnableForegroundMRC(bool enable);

	UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static void SetMRCTrackingReference(USceneComponent* TrackingReference);

	UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static void SetMRCUseCustomTrans(const FTransform& CustomTrans, bool UseCutomTrans);

	UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static void GetMRCRelativePose(FTransform& CustomTrans);
};
