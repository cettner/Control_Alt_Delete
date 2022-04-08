// Fill out your copyright notice in the Description page of Project Settings.


#include "HostSessionSettingsWidget.h"
#include "GameFramework/PlayerState.h"

bool UHostSessionSettingsWidget::Initialize()
{
    bool retval = Super::Initialize();

    if (PrivateGameCheckBox != nullptr)
    {
        PrivateGameCheckBox->SetIsChecked(false);
    }

    /*Password Edit */
    if (PasswordEditTextBox != nullptr)
    {
        const APlayerController * pc = GetOwningPlayer();
        if (pc != nullptr)
        {
            const APlayerState* ps = pc->PlayerState;
            const FString defaultservername = ps->GetPlayerName() += FString("'s Server");
            PasswordEditTextBox->SetText(FText::FromString(defaultservername));
        }

        if (PrivateGameCheckBox != nullptr)
        {
            const bool shouldenabletextbox = PrivateGameCheckBox->IsChecked();
            PasswordEditTextBox->SetIsEnabled(shouldenabletextbox);
        }

    }

    return retval;
}

void UHostSessionSettingsWidget::OnBeginHostingButtonPressed()
{
}
