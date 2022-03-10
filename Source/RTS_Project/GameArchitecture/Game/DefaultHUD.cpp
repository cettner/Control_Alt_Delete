// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"


void ADefaultHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PauseMenuClass != nullptr)
	{
		PauseMenu = CreateWidget(PlayerOwner, PauseMenuClass);
		PauseMenu->AddToViewport();
		PauseMenuEnable(false);
	}
}

bool ADefaultHUD::InitializeUI()
{
	const bool retval = InitPrimaryUI();
	if (retval == true)
	{
		GetPrimaryUI()->AddToViewport();
	}
	return retval;
}

UUserWidget* ADefaultHUD::GetPauseMenu() const
{
	return PauseMenu;
}

void ADefaultHUD::PauseMenuEnable(bool bInEnabled)
{
	if (IsValid(PauseMenu))
	{
		if (bInEnabled == true)
		{
			PauseMenu->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

bool ADefaultHUD::InitPrimaryUI()
{
	if (PrimaryUIClass == nullptr || MainUI != nullptr) return(false);
	MainUI = CreateWidget(GetOwningPlayerController(), PrimaryUIClass, "MainUI");
	const bool retval = MainUI != nullptr;
	return retval;
	
}

UUserWidget* ADefaultHUD::GetPrimaryUI() const
{
	return MainUI;
}
