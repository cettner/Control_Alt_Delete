// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"
#include "../GameArchitecture/LobbyPlayerController.h"
#include "../GameArchitecture/LobbyGameState.h"


void ULobbyMenu::Teardown()
{
	this->RemoveFromViewport();
}

void ULobbyMenu::DrawLobbySlots(TArray<FLobbyData> TeamSlots)
{
	UWorld* World = this->GetWorld();
	if (TeamAList == nullptr || TeamBList == nullptr || World == nullptr || TeamSlots.Num() < 2 ) return;

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
	StartGameButton->bIsEnabledDelegate.BindDynamic(this, &ULobbyMenu::CanStartGame);

	if (LeaveLobbyButton == nullptr) return false;
	LeaveLobbyButton->OnClicked.AddDynamic(this, &ULobbyMenu::OnPressedLeaveLobbyButton);

	AddToViewport();


	//ALobbyGameState* GS = Cast<ALobbyGameState>(World->GetGameState());
	
	/*Draw the initial team slots*/
	//DrawLobbySlots(GS->GetLobbyData());

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

bool ULobbyMenu::CanStartGame()
{
	bool retval = true;

	const UWorld* world = GetWorld();
	if (!IsValid(world))
	{
		const ALobbyPlayerController* pc = world->GetFirstPlayerController<ALobbyPlayerController>();

		/*Only the Lobby host can start the game*/
		if (IsValid(pc) && pc->HasAuthority())
		{
			retval = true;
		}
		else
		{
			retval = false;
		}
	}

	return retval;
}
