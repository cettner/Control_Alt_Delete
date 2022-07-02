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
					const FButtonDelegateBinding stitchfunction = GetWidgetBindFunctionHandler(menuwidget);
					transitionbutton->OnClicked.__Internal_AddDynamic(this, stitchfunction.GetMethod(), stitchfunction.GetName());
				}
			}
		}
	}
}

UMainMenuWidgetManager::FButtonDelegateBinding UMainMenuWidgetManager::GetWidgetBindFunctionHandler(const UWidget* InFindWidgetHandle) const
{
	/*Default Handle*/
	FButtonDelegateBinding retval;
	
	if (InFindWidgetHandle == MainMenuWidget)
	{
		retval = CREATE_DELEGATE_BINDING(&UMainMenuWidgetManager::DisplayMainMenu);
	}
	else if (InFindWidgetHandle == MultiplayerMenuWidget)
	{
		retval = CREATE_DELEGATE_BINDING(&UMainMenuWidgetManager::DisplayMultiPlayerMenu);
	}
	else if (InFindWidgetHandle == GameSettingsMenuWidget)
	{
		retval = CREATE_DELEGATE_BINDING(&UMainMenuWidgetManager::DisplayGameSettingsMenu);
	}
	else
	{
		retval = CREATE_DELEGATE_BINDING(&UMainMenuWidgetManager::DefaultMenuHandle);
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
