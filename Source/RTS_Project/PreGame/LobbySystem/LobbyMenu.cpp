// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"
#include "LobbyPlayerController.h"
#include "RTS_Project/GameArchitecture/Lobby/LobbyGameState.h"


void ULobbyMenu::Teardown()
{
	this->RemoveFromViewport();
}

void ULobbyMenu::DrawLobbySlots(TArray<FLobbyData> TeamSlots)
{
	UWorld* World = this->GetWorld();
	if (TeamAList == nullptr || TeamBList == nullptr || World == nullptr || TeamSlots.Num() < 2 ) return;
	
	World->GetFirstPlayerController<ALobbyPlayerController>();

	TeamAList->ClearChildren();
	TeamBList->ClearChildren();
	FLobbyData Ateam = TeamSlots[0];
	FLobbyData Bteam = TeamSlots[1];

	for( int i = 0; i < Ateam.TeamData.Num(); i++)
	{
		FSlotPlayerData data = Ateam.TeamData[i];
		ULobbySlotWidget* slot = CreateWidget<ULobbySlotWidget>(World, LobbySlotClass);
		if (slot)
		{
			slot->Setup(data);
			TeamAList->AddChild(slot);
		}	
	}

	for (int i = 0; i < Bteam.TeamData.Num(); i++)
	{
		FSlotPlayerData data = Bteam.TeamData[i];
		ULobbySlotWidget* slot = CreateWidget<ULobbySlotWidget>(World, LobbySlotClass);
		if (slot)
		{
			slot->Setup(data);
			TeamBList->AddChild(slot);
		}
	}
}

bool ULobbyMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (StartGameButton == nullptr) return false;
	StartGameButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnPressedStartGameButton);

	if (LeaveLobbyButton == nullptr) return false;
	LeaveLobbyButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnPressedLeaveLobbyButton);

	bIsFocusable = true;
	return(true);
}

void ULobbyMenu::OnPressedStartGameButton()
{
	UWorld* World = GetWorld();
	ALobbyPlayerController* PC = World->GetFirstPlayerController<ALobbyPlayerController>();
	if (PC == nullptr) return;
	PC->RequestStartGame();
}

void ULobbyMenu::OnPressedLeaveLobbyButton()
{
	ULobbyGameInstance * GI = GetGameInstance<ULobbyGameInstance>();

	if (GI != nullptr)
	{
		GI->EndSession();
	}
}

void ULobbyMenu::Setup()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ALobbyPlayerController* PC = World->GetFirstPlayerController<ALobbyPlayerController>();
	if (PC == nullptr) return;
	PC->SetLobbyMenu(this);


	ALobbyGameState* GS = Cast<ALobbyGameState>(World->GetGameState());
	if (GS == nullptr) return;

	/*Only the Lobby host can start the game*/
	if (PC->GetNetMode() == NM_ListenServer)
	{
		StartGameButton->SetIsEnabled(true);
	}
	else
	{
		StartGameButton->SetIsEnabled(false);
	}


	/*Draw the initial team slots*/
	DrawLobbySlots(GS->GetLobbyData());


	this->AddToViewport();

	// Set the Input Mode for the Player Controller as the UI only
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputModeData);
	PC->bShowMouseCursor = true;

}
