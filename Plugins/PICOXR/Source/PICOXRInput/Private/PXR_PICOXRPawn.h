// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PXR_PICOXRPawn.generated.h"

UCLASS()
class APXR_PICOXRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APXR_PICOXRPawn();
	
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float RightTriggerAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float RightGripAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float RightJoyStickXAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float RightJoyStickYAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float LeftTriggerAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float LeftGripAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float LeftJoyStickXAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	float LeftJoyStickYAxis;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool AButtonPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool BButtonPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool XButtonPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool YButtonPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool RightHomePressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool RightSystemPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool LeftHomePressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool LeftMenuPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool RightTriggerPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool RightGripPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool LeftTriggerPressed;
	UPROPERTY(BlueprintReadOnly, Category = "PICOPawn")
	bool LeftGripPressed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	void ClickButtonAPressed();
	void ClickButtonAReleased();
	void ClickButtonBPressed();
	void ClickButtonBReleased();
	
	void ClickButtonXPressed();
	void ClickButtonXReleased();
	void ClickButtonYPressed();
	void ClickButtonYReleased();
	
	void ClickButtonLeftHomePressed();
	void ClickButtonLeftHomeReleased();
	void ClickButtonLeftMenuPressed();
	void ClickButtonLeftMenuReleased();
	
	void ClickButtonRightHomePressed();
	void ClickButtonRightHomeReleased();
	void ClickButtonRightSystemPressed();
	void ClickButtonRightSystemReleased();
	
	void ClickButtonLeftTriggerPressed();
	void ClickButtonLeftTriggerReleased();
	void ClickButtonLeftGripPressed();
	void ClickButtonLeftGripReleased();
	
	void ClickButtonRightTriggerPressed();
	void ClickButtonRightTriggerReleased();
	void ClickButtonRightGripPressed();
	void ClickButtonRightGripReleased();
	
	void LeftTriggerAxisVal(float Val);
	void LeftGripAxisVal(float Val);
	void LeftJoyStickXVal(float Val);
	void LeftJoyStickYVal(float Val);
	
	void RightTriggerAxisVal(float Val);
	void RightGripAxisVal(float Val);
	void RightJoyStickXVal(float Val);
	void RightJoyStickYVal(float Val);


};
