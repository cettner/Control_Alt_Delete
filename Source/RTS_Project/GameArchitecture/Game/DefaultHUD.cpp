// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultHUD.h"


void ADefaultHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PauseMenuClass != nullptr)
	{
		PauseMenu = CreateWidget(PlayerOwner, PauseMenuClass);
	}
}

bool ADefaultHUD::ClientInitializeHUD()
{
	ProcessInputSettings(GetDefaultInputSettings());
	const bool retval = InitPrimaryUI();
	if (retval == true)
	{
		GetPrimaryUI()->AddToViewport();
	}
	return retval;
}

UUserWidget* ADefaultHUD::GetPauseMenu() const
{
	return PauseMenu;
}

FStackWidgetInfo ADefaultHUD::GetDefaultInputSettings() const
{
	return DefaultInputSettings;
}

void ADefaultHUD::ProcessInputSettings(const FStackWidgetInfo InWidgetinfo)
{
	SetMouseCursorVisible(InWidgetinfo.bIsCursorVisible, InWidgetinfo.CursorType);
	const EWisgetStackInputType inputtype = InWidgetinfo.InputType;

	if (inputtype == EWisgetStackInputType::GAMEONLY)
	{
		FInputModeGameOnly gameinput;
		SetInputMode(gameinput);
	}
	else if (inputtype == EWisgetStackInputType::UIONLY)
	{
		FInputModeUIOnly uiinput;
		SetInputMode(uiinput);
	}
	else
	{
		FInputModeGameAndUI gameanduiinput;
		SetInputMode(gameanduiinput);
	}

}

void ADefaultHUD::SetInputMode(const FInputModeDataBase& InData)
{
	GetOwningPlayerController()->SetInputMode(InData);
}

void ADefaultHUD::SetMouseCursorVisible(bool bIsCursorVisible, EMouseCursor::Type CursorType)
{
	GetOwningPlayerController()->bShowMouseCursor = bIsCursorVisible;
	GetOwningPlayerController()->CurrentMouseCursor = CursorType;
}

void ADefaultHUD::PauseMenuEnable(bool bInEnabled)
{
	if (IsValid(PauseMenu))
	{
		if (bInEnabled == true)
		{
			PauseMenu->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PauseMenu->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ADefaultHUD::PushExternalWidget(UUserWidget* InWidget)
{
	IExternalWidgetInterface * extwidget = Cast<IExternalWidgetInterface>(InWidget);
	FStackWidgetInfo inputsettings = FStackWidgetInfo();

	if (extwidget != nullptr)
	{
		inputsettings = extwidget->GetWidgetInputSettings();
	}
	else
	{
		inputsettings = GetDefaultInputSettings();
	}

	if (inputsettings.StackSettings == EWidgetStackOperation::ACIVEONPUSH)
	{
		ProcessInputSettings(inputsettings);
	}

	InWidget->AddToViewport();
	InWidget->SetFocus();
	ExternalWidgets.Emplace(InWidget);
}

bool ADefaultHUD::PopExternalWidget()
{
	bool retval = false;
	UUserWidget* widgettoremove = ExternalWidgets.Last();
	const bool isvalidwidget = IsValid(widgettoremove);
	FStackWidgetInfo inputsettings = FStackWidgetInfo();

	if(isvalidwidget == true)
	{
		const int indextoremove = ExternalWidgets.Num() - 1;
		widgettoremove->RemoveFromParent();
		ExternalWidgets.RemoveAt(indextoremove);

		retval = true;

		/*See if the next widget in the stack overrides input settings*/
		const IExternalWidgetInterface* nextwidget = nullptr;
		
		if (ExternalWidgets.Num() > 0)
		{
			nextwidget = Cast<IExternalWidgetInterface>(ExternalWidgets.Last());
		}


		if (nextwidget != nullptr)
		{
			const FStackWidgetInfo nextsettings = nextwidget->GetWidgetInputSettings();
			if (nextsettings.StackSettings >= EWidgetStackOperation::ACTIVEONPOPPED)
			{
				inputsettings = nextsettings;
			}
		}
		else
		{
			inputsettings = GetDefaultInputSettings();
		}
	}
	else
	{
		inputsettings = GetDefaultInputSettings();
	}


	ProcessInputSettings(inputsettings);


	return retval;
}

void ADefaultHUD::OnEscapeAction()
{
	const bool iswidgetopen = IsExternalMenuOpen();
	if (iswidgetopen == false && PauseMenu != nullptr)
	{
		PushExternalWidget(PauseMenu);
	}
	else
	{
		PopExternalWidget();
	}
}

uint32 ADefaultHUD::NumOpenWidgets() const
{
	return ExternalWidgets.Num();
}

bool ADefaultHUD::IsExternalMenuOpen() const
{
	return ExternalWidgets.Num() > 0;
}

bool ADefaultHUD::IsAnyMenuOpen() const
{
	const bool retval = ExternalWidgets.Num() > 0;
	return retval;
}

bool ADefaultHUD::InitPrimaryUI()
{
	if (PrimaryUIClass == nullptr || MainUI != nullptr) return(false);
	MainUI = CreateWidget(GetOwningPlayerController(), PrimaryUIClass, "MainUI");
	const bool retval = MainUI != nullptr;
	return retval;
	
}

UUserWidget* ADefaultHUD::GetPrimaryUI() const
{
	return MainUI;
}
