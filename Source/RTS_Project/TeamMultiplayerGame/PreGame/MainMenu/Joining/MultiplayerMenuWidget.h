// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"

#include "TabButtonWidget.h"
#include "../../../Interfaces/SessionMenuInterface.h"
#include "MultiplayerMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMultiplayerMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual bool Initialize() override;
	virtual void InitSessionInterface();

protected:
	UFUNCTION()
	virtual void OnHostSessionPressed();

	UFUNCTION()
	virtual void OnJoinSessionPressed();

protected:
	ISessionMenuInterface* GetSessionInterface() const;
	bool SetActiveWidgetTab(UWidget * InSetActive);
	bool SetActiveWidgetTab(UTabButtonWidget* InSetActive);

	UTabButtonWidget* GetActiveTabWidgetButton() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* SessionWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UTabButtonWidget* JoinSessionButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTabButtonWidget* HostSessionButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget * HostSessionMenu = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UWidget * JoinSessionMenu = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* TabContainer = nullptr;


protected:

	ISessionMenuInterface * SessionMenuInterface = nullptr;

	UTabButtonWidget* ActiveTabButton = nullptr;
};
