// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "../../../LobbyGameInstance.h"
#include "JoinSessionInfoWidget.h"
#include "JoinSessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UJoinSessionWidget : public UUserWidget
{
	GENERATED_BODY()
	


public:
	virtual bool IsSearchPending() const;

protected:
	virtual bool Initialize() override;
	virtual void InitSessionInterface();
	virtual bool StartSearch();

protected:
	UFUNCTION()
	virtual void OnSearchResultsRecieved(const FSessionSearchResults InSearchResults);

	UFUNCTION()
	virtual void OnRefreshButtonListButtonPressed();

	UFUNCTION()
	virtual bool ShouldEnableRefreshButton();

	UFUNCTION()
	virtual ESlateVisibility ShouldShowSearchingWidgets();

protected:

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* SessionSelectionPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* RefreshListButton;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UWidget* SearchInProgressWidget;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UJoinSessionInfoWidget> SessionInfoWidgetClass;

protected:
	ISessionMenuInterface* SessionInterface = nullptr;

};
