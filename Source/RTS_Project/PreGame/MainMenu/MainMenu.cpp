// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MainMenuPlayerController.h"
#include "Joining/MultiplayerMenuWidget.h"
#include "GameplaySettings/GameplaySettingsWidget.h"
#include "RTS_Project/LobbySystem/GameArchitecture/Instance/LobbyGameInstance.h"



void UMainMenu::InitMenuBindings()
{
	if (IsValid(MultiPlayerButton))
	{
		FMenuClickBindingInfo multiplayerbinding;
		multiplayerbinding.BindingDelegate = &MultiPlayerButton->OnClicked;
		multiplayerbinding.BindToClass = UMultiplayerMenuWidget::StaticClass();
		BindingInfo.Emplace(multiplayerbinding);
	}

	if (IsValid(GameSettingsButton))
	{
		FMenuClickBindingInfo gamesettingsbinding;
		gamesettingsbinding.BindingDelegate = &GameSettingsButton->OnClicked;
		gamesettingsbinding.BindToClass = UGameplaySettingsWidget::StaticClass();
		BindingInfo.Emplace(gamesettingsbinding);
	}
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	/*Setting this so that input can be set to UI only from controller*/
	//bIsFocusable = true;
	//AddToViewport();

	return true;
}

void UMainMenu::AddToScreen(ULocalPlayer * LocalPlayer, int32 ZOrder)
{
	Super::AddToScreen(LocalPlayer, ZOrder);
	const UWorld * world = GetWorld();

	/*Will return null in editor preview, valid in PIE*/
	APlayerController * pc = world->GetFirstPlayerController();
	if (pc != nullptr)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		pc->SetInputMode(InputModeData);
		pc->bShowMouseCursor = true;
	}
}





