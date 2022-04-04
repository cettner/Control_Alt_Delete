// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MainMenuPlayerController.h"
#include "RTS_Project/LobbySystem/GameArchitecture/Instance/LobbyGameInstance.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	/*Setting this so that input can be set to UI only from controller*/
	bIsFocusable = true;
	AddToViewport();


	return true;
}

void UMainMenu::AddToScreen(ULocalPlayer * LocalPlayer, int32 ZOrder)
{
	Super::AddToScreen(LocalPlayer, ZOrder);
	const UWorld * world = GetWorld();
	APlayerController * pc = world->GetFirstPlayerController();

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	pc->SetInputMode(InputModeData);
	pc->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	AMainMenuPlayerController* PlayerController = Cast<AMainMenuPlayerController>(World->GetFirstPlayerController());
	if (PlayerController == nullptr) return;

	// Set the Input Mode for game mode: allows only the player input / player controller to respond to user input.
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}


void UMainMenu::OnPlayOfflineButtonPressed()
{
	ULobbyGameInstance * GI = GetGameInstance<ULobbyGameInstance>();
	if (GI)
	{
		GI->StartOfflineGame();
	}
}



