// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectionCamera.h"
#include "RTSPlayerController.h"
#include "RTSHUD.h"
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

	if(PC)
	{
		ARTSHUD * HudPtr = Cast<ARTSHUD>(PC->GetHUD());
		
		FHitResult Hit;
		PC->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);
		AActor * target = Hit.GetActor();

		//save the result to check on release
		TempClick = Cast<ARTSSelectable>(target);

		if (CurrentView.GetSelectable() && CurrentView.GetSelectable() != TempClick)
		{
			CurrentView.GetSelectable()->SetDeselected();
			CurrentView.empty();
		}

		HudPtr->Initial_select = HudPtr->GetMouseLocation();
		HudPtr->SelctionInProcess = true;
	}
}

void ARTSSelectionCamera::SelectReleased()
{

}
	
void ARTSSelectionCamera::MoveSelected()
{

}