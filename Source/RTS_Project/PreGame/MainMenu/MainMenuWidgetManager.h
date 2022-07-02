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



#define CREATE_DELEGATE_BINDING(n) FButtonDelegateBinding(n, STATIC_FUNCTION_FNAME( TEXT(#n)))

struct FButtonDelegateBinding
{
public:
		FButtonDelegateBinding(){};

		FButtonDelegateBinding(TBaseDynamicDelegate<FWeakObjectPtr, void>::TMethodPtrResolver<UMainMenuWidgetManager>::FMethodPtr InMethod, FName InName)
		{
			Method = InMethod;
			FunctionName = InName;
		}
		
		TBaseDynamicDelegate<FWeakObjectPtr, void>::TMethodPtrResolver<UMainMenuWidgetManager>::FMethodPtr GetMethod() const
		{
			return Method;
		}

		FName GetName() const
		{
			return FunctionName;
		}

private:
		TBaseDynamicDelegate<FWeakObjectPtr, void>::TMethodPtrResolver<UMainMenuWidgetManager>::FMethodPtr Method;
		FName FunctionName;
};

public:
	UWidget* GetCurrentMenu() const;

protected:
	virtual bool Initialize() override;
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

protected:

	virtual void StitchMenuBindings();

	virtual FButtonDelegateBinding GetWidgetBindFunctionHandler(const UWidget* InFindWidgetHandle) const;


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
