// Fill out your copyright notice in the Description page of Project Settings.


#include "JoinSessionWidget.h"


bool UJoinSessionWidget::Initialize()
{
	bool retval = Super::Initialize();
	InitSessionInterface();
	retval &= SessionInterface != nullptr;

	ULobbyGameInstance* gi = GetGameInstance<ULobbyGameInstance>();
	if (IsValid(gi) && !gi->SearchResultsReadyDelegate.IsBound())
	{
		gi->SearchResultsReadyDelegate.BindUFunction(this, "OnSearchResultsRecieved");
	}

	if (IsValid(RefreshListButton))
	{
		RefreshListButton->OnClicked.AddDynamic(this, &UJoinSessionWidget::OnRefreshButtonListButtonPressed);
	}


	return retval;
}

void UJoinSessionWidget::InitSessionInterface()
{
	SessionInterface = GetGameInstance<ISessionMenuInterface>();
}

bool UJoinSessionWidget::StartSearch()
{
	bool retval = false;
	if (!IsSearchPending())
	{
		SessionSelectionPanel->ClearChildren();
		SessionInterface->BeginSearchQuery();
		retval = true;
	}
	return retval;
}

bool UJoinSessionWidget::IsSearchPending() const
{
	bool retval = false;

	if (SessionInterface != nullptr)
	{
		retval = SessionInterface->IsSearchingSession();
	}
	
	return retval;
}

void UJoinSessionWidget::OnSearchResultsRecieved(const FSessionSearchResults InSearchResults)
{
	for (int i = 0; i < InSearchResults.SearchResults.Num(); i++)
	{
		const FOnlineSessionSearchResult& currentresult = InSearchResults.SearchResults[i];

		if (currentresult.IsValid() && SessionInfoWidgetClass != nullptr)
		{
			UJoinSessionInfoWidget * sessioninfowidget = CreateWidget<UJoinSessionInfoWidget>(this, SessionInfoWidgetClass);
			sessioninfowidget->Setup(currentresult, i);
			SessionSelectionPanel->AddChild(sessioninfowidget);
		}

	}
}

void UJoinSessionWidget::OnRefreshButtonListButtonPressed()
{
	StartSearch();
}
