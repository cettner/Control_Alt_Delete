// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySlotWidget.h"
#include "GameArchitecture/Instance/LobbyGameInstance.h"

bool ULobbySlotWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (JoinSlotButton == nullptr) return false;
	JoinSlotButton->OnClicked.AddDynamic(this, &ULobbySlotWidget::OnJoinSlotButtonPressed);


	SlotData.isSlotActive = false;
	return (true);
}


void ULobbySlotWidget::Setup(FSlotPlayerData SlotInfo)
{
	SlotData = SlotInfo;
	if (SlotInfo.isSlotActive)
	{
		PlayerName->SetText(FText::FromString(SlotInfo.PlayerName));
		SlotActiveSwitcher->SetActiveWidget(FilledSlot);
	}
	else
	{
		SlotActiveSwitcher->SetActiveWidget(EmptySlot);
	}
}

void ULobbySlotWidget::OnJoinSlotButtonPressed()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ALobbyGameState* GS = World->GetGameState<ALobbyGameState>();
	if (GS == nullptr) return;

	ALobbyPlayerController* PC = World->GetFirstPlayerController<ALobbyPlayerController>();
	if (PC == nullptr) return;

	PC->RequestMoveSlot(SlotData);
}

