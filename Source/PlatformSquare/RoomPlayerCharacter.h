// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoomPlayerCharacter.generated.h"

UCLASS()
class PLATFORMSQUARE_API ARoomPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoomPlayerCharacter();

	

public:
	// Player's Pico id
	FString UserId;

	// Player's Pico display name
	FString DisplayName;

	// Dynamic material
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Implement by blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void CharacterMoveTo(FVector TargetLocation);

	// Set character material color
	void SetPlayerColor(FLinearColor InColor);

	void SetBaseInfo(const FString& InUserId, const FString& InDisplayName);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetUserDisplayName(){return DisplayName;}


};
