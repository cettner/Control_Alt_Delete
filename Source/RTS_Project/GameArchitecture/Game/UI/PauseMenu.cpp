// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "..\DefaultPlayerController.h"

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

    ReturnToLobbyButton->SetIsEnabled(GetOwningPlayer()->HasAuthority());

    return (Success);
}

FReply UPauseMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    FReply retval = FReply::Unhandled();

    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        OnResumePlayButtonPressed();
        retval = FReply::Handled();
    }

    return retval;
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
    ADefaultPlayerController * pc = GetOwningPlayer<ADefaultPlayerController>();
    pc->OnEndPause();
}

void UPauseMenu::OnReturnToLobbyButtonPressed()
{

}

void UPauseMenu::OnExitToMainMenuButtonPressed()
{

}