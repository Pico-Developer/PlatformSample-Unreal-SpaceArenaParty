// Fill out your copyright notice in the Description page of Project Settings.


#include "PXR_PICOXRPawn.h"
#include "PXR_HMDRuntimeSettings.h"


// Sets default values
APXR_PICOXRPawn::APXR_PICOXRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APXR_PICOXRPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APXR_PICOXRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}




// Called to bind functionality to input
void APXR_PICOXRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UPICOXRSettings* Settings = GetMutableDefault<UPICOXRSettings>();
	if(Settings->IsValidLowLevel()&&Settings->bEnableAutoKeyMapping)
	{
		// This component belongs to the possessing Player Controller
		InputComponent->BindAction("ButtonA", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonAPressed);
		InputComponent->BindAction("ButtonA", IE_Released, this, &APXR_PICOXRPawn::ClickButtonAReleased);

		InputComponent->BindAction("ButtonB", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonBPressed);
		InputComponent->BindAction("ButtonB", IE_Released, this, &APXR_PICOXRPawn::ClickButtonBReleased);

		InputComponent->BindAction("ButtonX", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonXPressed);
		InputComponent->BindAction("ButtonX", IE_Released, this, &APXR_PICOXRPawn::ClickButtonXReleased);

		InputComponent->BindAction("ButtonY", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonYPressed);
		InputComponent->BindAction("ButtonY", IE_Released, this, &APXR_PICOXRPawn::ClickButtonYReleased);

		InputComponent->BindAction("ButtonLeftGrip", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonLeftGripPressed);
		InputComponent->BindAction("ButtonLeftGrip", IE_Released, this, &APXR_PICOXRPawn::ClickButtonLeftGripReleased);

		InputComponent->BindAction("ButtonLeftTrigger", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonLeftTriggerPressed);
		InputComponent->BindAction("ButtonLeftTrigger", IE_Released, this, &APXR_PICOXRPawn::ClickButtonLeftTriggerReleased);


		InputComponent->BindAction("ButtonLeftHome", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonLeftHomePressed);
		InputComponent->BindAction("ButtonLeftHome", IE_Released, this, &APXR_PICOXRPawn::ClickButtonLeftHomeReleased);

		InputComponent->BindAction("ButtonLeftMenu", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonLeftMenuPressed);
		InputComponent->BindAction("ButtonLeftMenu", IE_Released, this, &APXR_PICOXRPawn::ClickButtonLeftMenuReleased);
		
		InputComponent->BindAction("ButtonRightGrip", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonRightGripPressed);
		InputComponent->BindAction("ButtonRightGrip", IE_Released, this, &APXR_PICOXRPawn::ClickButtonRightGripReleased);

		InputComponent->BindAction("ButtonRightTrigger", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonRightTriggerPressed);
		InputComponent->BindAction("ButtonRightTrigger", IE_Released, this, &APXR_PICOXRPawn::ClickButtonRightTriggerReleased);

		InputComponent->BindAction("ButtonRightHome", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonRightHomePressed);
		InputComponent->BindAction("ButtonRightHome", IE_Released, this, &APXR_PICOXRPawn::ClickButtonRightHomeReleased);

		InputComponent->BindAction("ButtonRightSystem", IE_Pressed, this, &APXR_PICOXRPawn::ClickButtonRightSystemPressed);
		InputComponent->BindAction("ButtonRightSystem", IE_Released, this, &APXR_PICOXRPawn::ClickButtonRightSystemReleased);


		
		InputComponent->BindAxis("LeftTriggerAxis", this, &APXR_PICOXRPawn::LeftTriggerAxisVal);
		InputComponent->BindAxis("LeftGripAxis", this, &APXR_PICOXRPawn::LeftGripAxisVal);

		InputComponent->BindAxis("RightTriggerAxis", this, &APXR_PICOXRPawn::RightTriggerAxisVal);
		InputComponent->BindAxis("RightGripAxis", this, &APXR_PICOXRPawn::RightGripAxisVal);

		InputComponent->BindAxis("LeftJoyStickXAxis", this, &APXR_PICOXRPawn::LeftJoyStickXVal);
		InputComponent->BindAxis("LeftJoyStickYAxis", this, &APXR_PICOXRPawn::LeftJoyStickYVal);
		
		InputComponent->BindAxis("RightJoyStickXAxis", this, &APXR_PICOXRPawn::RightJoyStickXVal);
		InputComponent->BindAxis("RightJoyStickYAxis", this, &APXR_PICOXRPawn::RightJoyStickYVal);
		
	}
}

void APXR_PICOXRPawn::ClickButtonAPressed()
{
	this->AButtonPressed=true;
}

void APXR_PICOXRPawn::ClickButtonAReleased()
{
	this->AButtonPressed=false;
}

void APXR_PICOXRPawn::ClickButtonBPressed()
{
	this->BButtonPressed=true;
}

void APXR_PICOXRPawn::ClickButtonBReleased()
{
	this->BButtonPressed=false;
}

void APXR_PICOXRPawn::ClickButtonXPressed()
{
	this->XButtonPressed=true;
}

void APXR_PICOXRPawn::ClickButtonXReleased()
{
	this->XButtonPressed=false;
}

void APXR_PICOXRPawn::ClickButtonYPressed()
{
	this->YButtonPressed=true;
}

void APXR_PICOXRPawn::ClickButtonYReleased()
{
	this->YButtonPressed=false;
}

void APXR_PICOXRPawn::ClickButtonLeftHomePressed()
{
	this->LeftHomePressed=true;
}

void APXR_PICOXRPawn::ClickButtonLeftHomeReleased()
{
	this->LeftHomePressed=false;
}

void APXR_PICOXRPawn::ClickButtonLeftMenuPressed()
{
	this->LeftMenuPressed=true;
}

void APXR_PICOXRPawn::ClickButtonLeftMenuReleased()
{
	this->LeftMenuPressed=false;
}

void APXR_PICOXRPawn::ClickButtonRightHomePressed()
{
	this->RightHomePressed=true;
}

void APXR_PICOXRPawn::ClickButtonRightHomeReleased()
{
	this->RightHomePressed=false;
}

void APXR_PICOXRPawn::ClickButtonRightSystemPressed()
{
	this->RightSystemPressed=true;
}

void APXR_PICOXRPawn::ClickButtonRightSystemReleased()
{
	this->RightSystemPressed=false;
}

void APXR_PICOXRPawn::ClickButtonLeftTriggerPressed()
{
	this->LeftTriggerPressed=true;
}

void APXR_PICOXRPawn::ClickButtonLeftTriggerReleased()
{
	this->LeftTriggerPressed=false;
}

void APXR_PICOXRPawn::ClickButtonLeftGripPressed()
{
	this->LeftGripPressed=true;
}

void APXR_PICOXRPawn::ClickButtonLeftGripReleased()
{
	this->LeftGripPressed=false;
}

void APXR_PICOXRPawn::ClickButtonRightTriggerPressed()
{
	this->RightTriggerPressed=true;
}

void APXR_PICOXRPawn::ClickButtonRightTriggerReleased()
{
	this->RightTriggerPressed=false;
}

void APXR_PICOXRPawn::ClickButtonRightGripPressed()
{
	this->RightGripPressed=true;
}

void APXR_PICOXRPawn::ClickButtonRightGripReleased()
{
	this->RightGripPressed=false;
}

void APXR_PICOXRPawn::LeftTriggerAxisVal(float Val)
{
	this->LeftTriggerAxis=Val;
}

void APXR_PICOXRPawn::LeftGripAxisVal(float Val)
{
	this->LeftGripAxis=Val;		
}

void APXR_PICOXRPawn::LeftJoyStickXVal(float Val)
{
	this->LeftJoyStickXAxis=Val;
}

void APXR_PICOXRPawn::LeftJoyStickYVal(float Val)
{
	this->LeftJoyStickYAxis=Val;
}

void APXR_PICOXRPawn::RightTriggerAxisVal(float Val)
{
	this->RightTriggerAxis=Val;
}

void APXR_PICOXRPawn::RightGripAxisVal(float Val)
{
	this->RightGripAxis=Val;
}

void APXR_PICOXRPawn::RightJoyStickXVal(float Val)
{
	this->RightJoyStickXAxis=Val;
}

void APXR_PICOXRPawn::RightJoyStickYVal(float Val)
{
	this->RightJoyStickYAxis=Val;
}


