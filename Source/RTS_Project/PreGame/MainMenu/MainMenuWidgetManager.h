// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "MainMenuWidgetManager.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMainMenuWidgetManager : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidget * MainMenuSelectorWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UWidget * GameSettingsMenuWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UWidget * MultiplayerMenuWidget;

};
