// Fill out your copyright notice in the Description page of Project Settings.


#include "HostSessionSettingsWidget.h"
#include "GameFramework/PlayerState.h"

bool UHostSessionSettingsWidget::Initialize()
{
    bool retval = Super::Initialize();
	InitSessionInterface();
	retval &= SessionInterface != nullptr;

	/*Set Up Default Server Name with the Players Session Username*/
	if (ServerNameEditTextBox != nullptr)
	{
		const FString defaultservername = GetDefaultServerName();
		ServerNameEditTextBox->SetText(FText::FromString(defaultservername));
	}


	/*Allow Enable Private Game*/
	if (PrivateGameCheckBox != nullptr)
	{
		PrivateGameCheckBox->SetIsChecked(false);
		PrivateGameCheckBox->OnCheckStateChanged.AddDynamic(this, &UHostSessionSettingsWidget::OnPrivateGameCheckStateChanged);
	}

    /*Password Edit */
    if (PasswordEditTextBox != nullptr)
    {
		PasswordEditTextBox->SetIsPassword(true);

        if (PrivateGameCheckBox != nullptr)
        {
            const bool shouldenabletextbox = PrivateGameCheckBox->IsChecked();
            PasswordEditTextBox->SetIsEnabled(shouldenabletextbox);
        }
    }

	if (BeginHostingButton != nullptr)
	{
		BeginHostingButton->OnClicked.AddDynamic(this, &UHostSessionSettingsWidget::OnBeginHostingButtonPressed);
	}

    return retval;
}

void UHostSessionSettingsWidget::OnBeginHostingButtonPressed()
{
	const FString servername = GetServerName();
	SessionInterface->Host(servername);
}

void UHostSessionSettingsWidget::OnPrivateGameCheckStateChanged(bool InState)
{
	if (InState == true)
	{
		PasswordEditTextBox->SetIsEnabled(true);
	}
	else
	{
		PasswordEditTextBox->SetIsEnabled(false);
	}
}

void UHostSessionSettingsWidget::InitSessionInterface()
{
	SessionInterface = GetGameInstance<ISessionMenuInterface>();
}

ISessionMenuInterface* UHostSessionSettingsWidget::GetSessionInterface() const
{
	return SessionInterface;
}

FString UHostSessionSettingsWidget::GetServerName() const
{
	FString retval;
	
	if (IsValid(ServerNameEditTextBox))
	{
		retval = ServerNameEditTextBox->GetText().ToString();
	}
	else
	{
		retval = GetDefaultServerName();
	}

	return retval;
}

FString UHostSessionSettingsWidget::GetDefaultServerName() const
{
	/*HardCoded Value as a Failsafe*/
	FString retval = DefaultServerName;

	/*Get the Player's handlename and name the server based on that*/
	const APlayerController* pc = GetOwningPlayer();
	if (pc != nullptr)
	{
		const APlayerState* ps = pc->PlayerState;
		const FString defaultservername = ps->GetPlayerName() += FString("'s Server");
		retval = defaultservername;
	}

	return retval;
}
