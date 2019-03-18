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
	ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetController());

	if(PC)
	{
		FHitResult Hit;
		PC->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);
		AActor * target = Hit.GetActor();
		ARTSSelectable * ReleaseClick = Cast<ARTSSelectable>(target);

		ARTSHUD * HudPtr = Cast<ARTSHUD>(PC->GetHUD());
		HudPtr->SelctionInProcess = false;
		HudPtr->Selected_Structure.Empty();

		SelectedStructures.Empty();
		SelectedUnits = HudPtr->Selected_Units;

		if (ReleaseClick && TempClick == ReleaseClick && TempClick != CurrentView.GetSelectable())
		{
			CurrentView.set(TempClick);
			CurrentView.GetSelectable()->SetSelected();
		}
		else if (SelectedUnits.Num())
		{
			CurrentView.set(SelectedUnits[0]);
		}
		else
		{
			CurrentView.empty();
		}

		TempClick = nullptr;
		PC->Update_UI_Selection();
	}
}
	
void ARTSSelectionCamera::MoveSelected()
{
	ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetController());
	if (SelectedUnits.Num() > 0  && PC)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			FHitResult Hit;
			PC->GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);
			AActor * target = Hit.GetActor();

			if (target)
			{
				SelectedUnits[i]->SetTarget(target);
			}
			/*
			FVector MoveLocal = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

			if (SelectedUnits[i]->HasAssets())
			{
				SelectedUnits[i]->ReleaseAssets(MoveLocal);
			}
			else
			{
				SelectedUnits[i]->RtsMove(MoveLocal);
			}
			*/
		}
	}
}