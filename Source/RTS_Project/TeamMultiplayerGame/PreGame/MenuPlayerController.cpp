// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"
#include "../LobbyGameInstance.h"

void AMenuPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (IsLocalPlayerController())
	{
		FInputModeUIOnly inputmodedata;
		inputmodedata.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(inputmodedata);
		bShowMouseCursor = true;
	}

}

void AMenuPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
	
	ULobbyGameInstance * gi = GetGameInstance<ULobbyGameInstance>();
	gi->StartLoadingLevel(FURL(*PendingURL), TravelType, bIsSeamlessTravel);
}
