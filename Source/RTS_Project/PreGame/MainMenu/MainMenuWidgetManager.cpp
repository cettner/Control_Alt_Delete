// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidgetManager.h"
#include "Delegates/Delegate.h"
#include "UObject/WeakObjectPtr.h"
#include "Delegates/DelegateSignatureImpl.inl"

bool UMainMenuWidgetManager::Initialize()
{
	bool retval = Super::Initialize();
	#if !WITH_EDITOR
		StitchMenuBindings();
	#endif
	DisplayMainMenu();
	AddToViewport();

	return retval;
}

void UMainMenuWidgetManager::AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder)
{
	Super::AddToScreen(LocalPlayer, ZOrder);
	const UWorld* world = GetWorld();

	/*Will return null in editor preview, valid in PIE*/
	APlayerController* pc = world->GetFirstPlayerController();
	if (pc != nullptr)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

		pc->SetInputMode(InputModeData);
		pc->bShowMouseCursor = true;
	}
}

void UMainMenuWidgetManager::StitchMenuBindings()
{
	if (IsValid(MainMenuWidget))
	{
		TArray<FMenuClickBindingInfo> mainbindinfo = MainMenuWidget->GetMenuBindings();
		TArray<UWidget*>  allmenus = MenuWidgetSwitcher->GetAllChildren();
		
		for (int i = 0; i < mainbindinfo.Num(); i++)
		{
			FMenuClickBindingInfo currentbinding = mainbindinfo[i];

			for (int k = 0; k < allmenus.Num(); k++)
			{
				const UWidget* menuwidget = allmenus[k];
				if (menuwidget->IsA(currentbinding.BindToClass))
				{
					UButton * transitionbutton = currentbinding.BindingButton;
					/*Typically dont use the "auto" keyword but the type declaratiion is very long for a simple function adress..*/
					const auto stitchfunction = GetWidgetBindFunctionHandler(menuwidget);
					transitionbutton->OnClicked.AddDynamic(this, &UMainMenuWidgetManager::DisplayMultiPlayerMenu);
					int debug = 9;
				}
			}
		}
	}
}

TBaseDynamicDelegate<FWeakObjectPtr, void>::TMethodPtrResolver<UMainMenuWidgetManager>::FMethodPtr UMainMenuWidgetManager::GetWidgetBindFunctionHandler(const UWidget* InFindWidgetHandle) const
{
	/*Default Handle*/
	TBaseDynamicDelegate<FWeakObjectPtr, void>::TMethodPtrResolver<UMainMenuWidgetManager>::FMethodPtr retval = &UMainMenuWidgetManager::DefaultMenuHandle;
	
	if (InFindWidgetHandle == MainMenuWidget)
	{
		retval = &UMainMenuWidgetManager::DisplayMainMenu;
	}
	else if (InFindWidgetHandle == MultiplayerMenuWidget)
	{
		retval = &UMainMenuWidgetManager::DisplayMultiPlayerMenu;
	}
	else if (InFindWidgetHandle == GameSettingsMenuWidget)
	{
		retval = &UMainMenuWidgetManager::DisplayGameSettingsMenu;
	}
	return retval;
}

void UMainMenuWidgetManager::DisplayMainMenu()
{
	/*Editor Protection*/
	if (MainMenuWidget != nullptr)
	{
		MenuWidgetSwitcher->SetActiveWidget(MainMenuWidget);
	}
}

void UMainMenuWidgetManager::DisplayGameSettingsMenu()
{
}

void UMainMenuWidgetManager::DisplayMultiPlayerMenu()
{
	if (MultiplayerMenuWidget != nullptr)
	{
		MenuWidgetSwitcher->SetActiveWidget(MultiplayerMenuWidget);
	}
}

void UMainMenuWidgetManager::DefaultMenuHandle()
{
}

UWidget* UMainMenuWidgetManager::GetCurrentMenu() const
{
	UWidget* retval = nullptr;
	if (IsValid(MenuWidgetSwitcher))
	{
		retval = MenuWidgetSwitcher->GetActiveWidget();
	}

	return retval;
}
