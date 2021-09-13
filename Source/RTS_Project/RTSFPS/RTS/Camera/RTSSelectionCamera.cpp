// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectionCamera.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSPlayerController.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSHUD.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"



// Called to bind functionality to input
void ARTSSelectionCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ARTSSelectionCamera::SelectPressed);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &ARTSSelectionCamera::SelectReleased);
	InputComponent->BindAction("RightMouse", IE_Pressed, this, &ARTSSelectionCamera::MoveSelected);
}

void  ARTSSelectionCamera::SelectPressed()
{
		ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetController());

		ARTSHUD * HudPtr = Cast<ARTSHUD>(PC->GetHUD());
		
		FHitResult Hit;
		PC->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);

		HudPtr->Initial_select = HudPtr->GetMouseLocation();
		HudPtr->SelctionInProcess = true;
}

void ARTSSelectionCamera::SelectReleased()
{
		ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetController());

		ARTSHUD * HudPtr = Cast<ARTSHUD>(PC->GetHUD());
		HudPtr->SelctionInProcess = false;
		HudPtr->Selected_Structure.Empty();

		SelectedStructures.Empty();
		SelectedUnits = HudPtr->Selected_Units;
}
	
void ARTSSelectionCamera::MoveSelected()
{
	ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetController());
	if (SelectedUnits.Num() > 0  && PC)
	{
		FHitResult Hit;
		PC->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);
		PC->MoveMinions(PC, SelectedUnits, Hit);
	}
}