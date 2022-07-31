// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerMenuWidget.h"

bool UMultiplayerMenuWidget::Initialize()
{
	bool success = Super::Initialize();

	InitSessionInterface();
	/*In Editor, this will fail since there's no gameinstance or session per player*/
	#if !WITH_EDITOR
		success &= SessionMenuInterface != nullptr;
	#endif

	/*Required Binding*/
	if (HostSessionButton == nullptr || HostSessionButton->GetButton() == nullptr) return false;
	HostSessionButton->GetButton()->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnHostSessionPressed);
	HostSessionButton->SetBoundWidget(HostSessionMenu);
	SetActiveWidgetTab(HostSessionButton);

	/*Optional Binding*/
	if (JoinSessionButton != nullptr && JoinSessionButton->GetButton() != nullptr)
	{
		JoinSessionButton->GetButton()->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnJoinSessionPressed);
		JoinSessionButton->SetBoundWidget(JoinSessionMenu);
	}


	return success;
}

void UMultiplayerMenuWidget::InitSessionInterface()
{
	SessionMenuInterface = GetGameInstance<ISessionMenuInterface>();
}

void UMultiplayerMenuWidget::OnHostSessionPressed()
{
	SetActiveWidgetTab(HostSessionButton);

}

void UMultiplayerMenuWidget::OnJoinSessionPressed()
{
	SetActiveWidgetTab(JoinSessionButton);
}

ISessionMenuInterface * UMultiplayerMenuWidget::GetSessionInterface() const
{
	return SessionMenuInterface;
}

bool UMultiplayerMenuWidget::SetActiveWidgetTab(UWidget* InSetActive)
{
	bool retval = false;

	if (IsValid(SessionWidgetSwitcher) && IsValid(InSetActive))
	{
		SessionWidgetSwitcher->SetActiveWidget(InSetActive);
		retval = true;
	}

	return retval;
}

bool UMultiplayerMenuWidget::SetActiveWidgetTab(UTabButtonWidget* InSetActive)
{
	bool retval = false;
	UTabButtonWidget* currenttabbutton = GetActiveTabWidgetButton();

	if (IsValid(InSetActive) && (currenttabbutton != InSetActive))
	{
		UWidget* buttonbinding = InSetActive->GetBoundWidget();
		retval = SetActiveWidgetTab(buttonbinding);
	}
	
	/**/
	if (retval == true)
	{
		if (IsValid(currenttabbutton))
		{
			currenttabbutton->SetActive(false);
		}
		InSetActive->SetActive(true);
		ActiveTabButton = InSetActive;
	}
		
	return retval;
}

UTabButtonWidget* UMultiplayerMenuWidget::GetActiveTabWidgetButton() const
{
	return ActiveTabButton;
}
