// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "MainMenuPlayerController.h"
#include "RTS_Project/LobbySystem/GameArchitecture/Instance/LobbyGameInstance.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;


	if (NewSessionButton == nullptr) return false;
	NewSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnNewSessionPressed);

	if (JoinSessionButton == nullptr) return false;
	JoinSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinSessionPressed);


	if (CancelJoinSessionButton == nullptr) return false;
	CancelJoinSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnCancelJoinSession);

	if (JoinSelectedSessionButton == nullptr) return false;
	JoinSelectedSessionButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinSelectedSession);

	if (PlayOfflineButton != nullptr)
	{
		PlayOfflineButton->OnClicked.AddDynamic(this, &UMainMenu::OnPlayOfflineButtonPressed);
	}



	/*Setting this so that input can be set to UI only from controller*/
	bIsFocusable = true;

	return true;
}

void UMainMenu::Setup(ISessionMenuInterface* MenuInterface)
{
	SessionMenuInterface = MenuInterface;

	this->AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	AMainMenuPlayerController* PC = Cast<AMainMenuPlayerController>(World->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}

	// Set the Input Mode for the Player Controller as the UI only
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputModeData);
	PC->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	AMainMenuPlayerController* PlayerController = Cast<AMainMenuPlayerController>(World->GetFirstPlayerController());
	if (PlayerController == nullptr) return;

	// Set the Input Mode for game mode: allows only the player input / player controller to respond to user input.
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu::OnNewSessionPressed()
{
	if (SessionMenuInterface == nullptr) return;
	SessionMenuInterface->Host(DesiredServerName);
}

void UMainMenu::OnJoinSessionPressed()
{
	if ((MenuSwitcher == nullptr) || (SessionListMenuWidget == nullptr)) return;

	MenuSwitcher->SetActiveWidget(SessionListMenuWidget);

	if (SessionMenuInterface == nullptr) return;

	SessionMenuInterface->OpenSessionListMenu();
}

void UMainMenu::OnPlayOfflineButtonPressed()
{
	ULobbyGameInstance * GI = GetGameInstance<ULobbyGameInstance>();
	if (GI)
	{
		GI->StartOfflineGame();
	}
}

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
