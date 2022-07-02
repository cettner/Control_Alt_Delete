// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		FInputModeUIOnly inputmodedata;
		//InputModeData.SetWidgetToFocus(this->TakeWidget());
		inputmodedata.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
		SetInputMode(inputmodedata);
		bShowMouseCursor = true;
	}

}
