// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"

bool ULobbyMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	bIsFocusable = true;
	return(true);
}

void ULobbyMenu::Setup()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC == nullptr)
	{
		return;
	}

	if (PC->GetNetMode() == NM_ListenServer)
	{
		StartGameButton->SetIsEnabled(true);
	}
	else
	{
		StartGameButton->SetIsEnabled(false);
	}

	this->AddToViewport();

	// Set the Input Mode for the Player Controller as the UI only
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputModeData);
	PC->bShowMouseCursor = true;

}
