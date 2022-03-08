// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"

bool UPauseMenu::Initialize()
{
    bool Success = Super::Initialize();

    if (!Success) return false;
	
	if (ResumePlayButton == nullptr) return false;
    ResumePlayButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumePlayButtonPressed);

	if (ReturnToLobbyButton == nullptr) return false;
    ReturnToLobbyButton->OnClicked.AddDynamic(this, &UPauseMenu::OnReturnToLobbyButtonPressed);

	if (ExitToMainMenuButton == nullptr) return false;
    ExitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnExitToMainMenuButtonPressed);


    return (true);
}


void UPauseMenu::OnResumePlayButtonPressed()
{

}


void UPauseMenu::OnReturnToLobbyButtonPressed()
{

}

void UPauseMenu::OnExitToMainMenuButtonPressed()
{

}