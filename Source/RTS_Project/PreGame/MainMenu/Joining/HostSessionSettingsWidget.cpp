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
	FSessionSettings hostsettings = DefaultSessionSettings;

	if (BuildHostSettings(hostsettings))
	{
		SessionInterface->Host("SESSION_NAME", hostsettings);
	}
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

void UHostSessionSettingsWidget::InitSessionSettings()
{
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

bool UHostSessionSettingsWidget::IsPasswordProtected() const
{
	bool retval = false;
	if (PrivateGameCheckBox != nullptr)
	{
		retval = PrivateGameCheckBox->IsChecked();
	}
	else if (PasswordEditTextBox != nullptr)
	{
		retval = PasswordEditTextBox->GetIsEnabled();
	}

	return retval;
}

FString UHostSessionSettingsWidget::GetPassword() const
{
	FString retval = "";
	if (IsValid(PasswordEditTextBox))
	{
		retval = PasswordEditTextBox->GetText().ToString();
	}

	return retval;
}

bool UHostSessionSettingsWidget::BuildHostSettings(FSessionSettings& OutSettings) const
{
	const FName ServerNameKey = "ServerName";
	FOnlineSessionSetting servernamesetting;
	servernamesetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineServiceAndPing;
	servernamesetting.Data = GetServerName();

	const FName IsPrivateGameKey = "IsPasswordProtected";
	FOnlineSessionSetting haspasswordsetting;
	haspasswordsetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineServiceAndPing;
	haspasswordsetting.Data = IsPasswordProtected();

	const FName PasswordKey = "Password";
	FOnlineSessionSetting passwordsetting;
	passwordsetting.AdvertisementType = EOnlineDataAdvertisementType::DontAdvertise;
	passwordsetting.Data = GetPassword();

	OutSettings.Add(ServerNameKey, servernamesetting);
	OutSettings.Add(IsPrivateGameKey, haspasswordsetting);
	OutSettings.Add(PasswordKey, passwordsetting);


	/*Todo:: add Setting Validation*/
	return true;
}
