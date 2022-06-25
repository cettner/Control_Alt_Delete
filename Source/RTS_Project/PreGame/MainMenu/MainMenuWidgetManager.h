// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"

#include "MainMenu.h"
#include "GameplaySettings/GameplaySettingsWidget.h"
#include "Joining/MultiplayerMenuWidget.h"
#include "MainMenuWidgetManager.generated.h"




UCLASS()
class RTS_PROJECT_API UMainMenuWidgetManager : public UUserWidget
{
	GENERATED_BODY()

public:
	UWidget* GetCurrentMenu() const;

protected:
	//virtual void DisplayMainMenu();

protected:
	virtual bool Initialize() override;

protected:

	virtual void StitchMenuBindings();

	virtual TBaseDynamicDelegate<FWeakObjectPtr, void>::TMethodPtrResolver<UMainMenuWidgetManager>::FMethodPtr GetWidgetBindFunctionHandler(UWidget* InFindWidgetHandle) const;

	UFUNCTION()
	virtual void DisplayMainMenu();

	UFUNCTION()
	virtual void DisplayGameSettingsMenu();

	UFUNCTION()
	virtual void DisplayMultiPlayerMenu();

	UFUNCTION()
	virtual void DefaultMenuHandle();

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuWidgetSwitcher = nullptr;

	UPROPERTY(meta = (BindWidget))
	UMainMenu* MainMenuWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UGameplaySettingsWidget * GameSettingsMenuWidget;

	UPROPERTY(meta = (BindWidgetOptional))
	UMultiplayerMenuWidget * MultiplayerMenuWidget;


};
