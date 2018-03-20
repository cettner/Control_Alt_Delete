// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"

#include "ConstructorHelpers.h"

ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

}

void ARTSPlayerController::BeginPlay()
{
	HudPtr = Cast<ARTSHUD>(GetHUD());  // GetHud only returns AHUD, need to cast to get child type, ARTSHUD
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ARTSPlayerController::SelectPressed);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &ARTSPlayerController::SelectReleased);
	InputComponent->BindAction("RightMouse", IE_Pressed, this, &ARTSPlayerController::MoveSelected);
}

void ARTSPlayerController::SelectPressed()
{
	HudPtr->Initial_select = HudPtr->GetMouseLocation();
	HudPtr->SelctionInProcess = true;
}

void ARTSPlayerController::SelectReleased()
{
	HudPtr->SelctionInProcess = false;
}

void ARTSPlayerController::MoveSelected()
{

}