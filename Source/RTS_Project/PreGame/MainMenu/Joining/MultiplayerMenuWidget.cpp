// Fill out your copyright notice in the Description page of Project Settings.


#include "RTS_Project/PreGame/MainMenu/Joining/MultiplayerMenuWidget.h"

bool UMultiplayerMenuWidget::Initialize()
{
	bool success = Super::Initialize();

	InitSessionInterface();
	/*In Editor, If we're not in PIE skip delegate bindings*/
	#if !WITH_EDITOR
		success &= SessionMenuInterface != nullptr;
	#endif

	/*Required Binding*/
	if (HostSessionButton == nullptr || HostSessionButton->GetButton() == nullptr) return false;
	HostSessionButton->GetButton()->OnClicked.AddDynamic(this, &UMultiplayerMenuWidget::OnHostSessionPressed);
	HostSessionButton->SetBoundWidget(HostSessionMenu);

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
	if (SetActiveWidgetTab(HostSessionButton))
	{
		HostSessionButton->SetActive(true);
	}

}

void UMultiplayerMenuWidget::OnJoinSessionPressed()
{
	if (SetActiveWidgetTab(JoinSessionButton))
	{
		HostSessionButton->SetActive(false);
	}
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
	if (IsValid(InSetActive))
	{
		UWidget* buttonbinding = InSetActive->GetBoundWidget();
		retval = SetActiveWidgetTab(buttonbinding);
	}
	
		
	return retval;
}


/*
void UMainMenu::OnCancelJoinSession()
{
	if ((MenuSwitcher == nullptr) || (HostSessionMenuWidget == nullptr)) return;

	MenuSwitcher->SetActiveWidget(HostSessionMenuWidget);
}

void UMainMenu::OnJoinSelectedSession()
{
	if ((ScrollSessionList == nullptr) && (SessionMenuInterface == nullptr)) return;

	if (SelectedScrollIndex.IsSet())
	{
		int32 ScrollCount = ScrollSessionList->GetChildrenCount();
		int32 SelectedIndex = (int32)SelectedScrollIndex.GetValue();
		if ((ScrollCount > 0) && (SelectedIndex >= 0) && (SelectedIndex < ScrollCount))
		{
			SessionMenuInterface->JoinSession(SelectedScrollIndex.GetValue());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::InitializeSessionsList] No sessions available"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::InitializeSessionsList] Unable to Join Session"));
	}
}

void UMainMenu::InitializeSessionsList(TArray<FServerData> Data)
{
	UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::InitializeSessionsList] %i"), Data.Num());
	if (ScrollSessionList == nullptr) return;

	UWorld* World = this->GetWorld();
	if (World == nullptr) return;

	ScrollSessionList->ClearChildren();
	uint32 IndexRow = 0;
	for (const FServerData& ServerData : Data)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::InitializeSessionsList] %s"), *ServerData.Name);

		UJoinSessionInfoWidget * Row = CreateWidget<UJoinSessionInfoWidget>(World, SessionRowClass);
		if (Row == nullptr) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));

		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));

		Row->Setup(this, IndexRow);
		++IndexRow;
		ScrollSessionList->AddChild(Row);
	}
}

void UMainMenu::SelectIndexSessionList(uint32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("[UMainMenu::SelectIndex] SelectIndex: %i"), Index);

	SelectedScrollIndex = Index;
	UpdateSessionList();
}

void UMainMenu::UpdateSessionList()
{
	if (ScrollSessionList == nullptr) return;

	// Start from 1, not counting Header
	int indexRow = 0;
	for (int32 i = 1; i < ScrollSessionList->GetChildrenCount(); ++i)
	{
		UJoinSessionInfoWidget * Row = Cast<UJoinSessionInfoWidget>(ScrollSessionList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedScrollIndex.IsSet() && (SelectedScrollIndex.GetValue() == indexRow));

			indexRow++;
		}
	}
}
*/