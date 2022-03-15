// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "..\DefaultPlayerController.h"

bool UPauseMenu::Initialize()
{
    bool Success = Super::Initialize();

    if (!Success) return false;
	
    DefaultInputSettings.bIsCursorVisible = true;
    DefaultInputSettings.CursorType = EMouseCursor::Hand;
    DefaultInputSettings.StackSettings = EWidgetStackOperation::ACIVEONPUSH;
    DefaultInputSettings.InputType = EWisgetStackInputType::UIONLY;

	if (ResumePlayButton == nullptr) return false;
    ResumePlayButton->OnClicked.AddDynamic(this, &UPauseMenu::OnResumePlayButtonPressed);

	if (ReturnToLobbyButton == nullptr) return false;
    ReturnToLobbyButton->OnClicked.AddDynamic(this, &UPauseMenu::OnReturnToLobbyButtonPressed);

	if (ExitToMainMenuButton == nullptr) return false;
    ExitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnExitToMainMenuButtonPressed);

    // ReturnToLobbyButton->SetIsEnabled(GetOwningPlayer()->HasAuthority());

    return (Success);
}

void UPauseMenu::SetVisibility(ESlateVisibility InVisibility)
{
    Super::SetVisibility(InVisibility);

    if (InVisibility == ESlateVisibility::Visible)
    {
        bIsFocusable = true;
        SetFocus();
    }
    else
    {
        bIsFocusable = false;
    }
}

void UPauseMenu::SetResumeHotKey(const FName ActionName)
{
    FName oldhotkey = ResumeHotKey;
    ResumeHotKey = ActionName;
}

void UPauseMenu::OnResumePlayButtonPressed()
{
    OnEscapeAction();
}

void UPauseMenu::OnReturnToLobbyButtonPressed()
{

}

void UPauseMenu::OnExitToMainMenuButtonPressed()
{

}