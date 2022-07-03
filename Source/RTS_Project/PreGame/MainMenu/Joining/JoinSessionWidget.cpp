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
	int debug = 9;
}

void UJoinSessionWidget::OnRefreshButtonListButtonPressed()
{
	StartSearch();
}
