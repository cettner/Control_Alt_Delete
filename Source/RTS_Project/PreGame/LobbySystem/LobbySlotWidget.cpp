// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbySlotWidget.h"

bool ULobbySlotWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	bIsSlotFilled = false;
	return (true);
}


void ULobbySlotWidget::Setup(FSlotPlayerData SlotInfo)
{
	if (SlotInfo.isSlotActive)
	{

	}
	else
	{

	}
}

void ULobbySlotWidget::ActivateSlot(FSlotPlayerData SlotInfo)
{
	if ((SlotActiveSwitcher == nullptr) || (FilledSlot == nullptr))
	{
		return;
	}
	bIsSlotFilled = true;
}

void ULobbySlotWidget::DeactivateSlot()
{
	if ((SlotActiveSwitcher == nullptr) || (EmptySlot == nullptr))
	{
		return;
	}
	bIsSlotFilled = false;
}

