// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Joining/MultiplayerMenuWidget.h"
#include "GameplaySettings/GameplaySettingsWidget.h"



void UMainMenu::InitMenuBindings()
{
	if (IsValid(MultiPlayerButton))
	{
		FMenuClickBindingInfo multiplayerbinding;
		multiplayerbinding.BindingButton = MultiPlayerButton;
		multiplayerbinding.BindToClass = UMultiplayerMenuWidget::StaticClass();
		BindingInfo.Emplace(multiplayerbinding);
	}

	if (IsValid(GameSettingsButton))
	{
		FMenuClickBindingInfo gamesettingsbinding;
		gamesettingsbinding.BindingButton = GameSettingsButton;
		gamesettingsbinding.BindToClass = UGameplaySettingsWidget::StaticClass();
		BindingInfo.Emplace(gamesettingsbinding);
	}
}
