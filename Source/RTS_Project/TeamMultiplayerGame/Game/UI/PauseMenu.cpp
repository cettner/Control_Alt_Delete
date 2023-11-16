// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "..\GameArchitecture\DefaultMode.h"

UPauseMenu::UPauseMenu() : Super()
{
    SetIsFocusable(true);
    SetKeyboardFocus();
}

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

    bool hasservercontrol = false;
    const APlayerController* pc = GetOwningPlayer();
    if (pc && pc->HasAuthority())
    {
        hasservercontrol = true;
    }
    ReturnToLobbyButton->SetIsEnabled(hasservercontrol);

	if (ExitToMainMenuButton == nullptr) return false;
    ExitToMainMenuButton->OnClicked.AddDynamic(this, &UPauseMenu::OnExitToMainMenuButtonPressed);

    return (Success);
}

FReply UPauseMenu::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    FReply retval = FReply::Handled();

    if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        OnResumePlayButtonPressed();
    }
    return retval;
}

void UPauseMenu::OnResumePlayButtonPressed()
{
    OnEscapeAction();
}

void UPauseMenu::OnReturnToLobbyButtonPressed()
{
    const UWorld* world = GetWorld();
    ADefaultMode * gm = world->GetAuthGameMode<ADefaultMode>();

    /*This will return null on clients anyway so no need to check for authority*/
    if (IsValid(gm))
    {
        gm->EndMatch();
    }
    
}

void UPauseMenu::OnExitToMainMenuButtonPressed()
{

}