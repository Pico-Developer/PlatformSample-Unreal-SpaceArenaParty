// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomPlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARoomPlayerCharacter::ARoomPlayerCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));


}

// Called when the game starts or when spawned
void ARoomPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
    DynamicMaterialInstance = GetMesh()->CreateDynamicMaterialInstance(0);
   // FLinearColor RandomColor = FLinearColor(FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), 1.f);


}

// Called every frame
void ARoomPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomPlayerCharacter::SetPlayerColor(FLinearColor InColor)
{
    if (DynamicMaterialInstance)
    {
        DynamicMaterialInstance->SetVectorParameterValue(FName(TEXT("BodyColor")), InColor);
    }
}

void ARoomPlayerCharacter::SetBaseInfo(const FString& InUserId, const FString& InDisplayName)
{
    UserId = InUserId;
    DisplayName = InDisplayName;
}

