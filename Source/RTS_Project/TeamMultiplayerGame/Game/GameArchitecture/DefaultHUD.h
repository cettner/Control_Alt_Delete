// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../UI/Interfaces/ExternalWidgetInterface.h"
#include "DefaultGameState.h"
#include "DefaultHUD.generated.h"




UCLASS()
class RTS_PROJECT_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()

	protected:
		virtual void PostInitializeComponents() override;

	public:
		virtual bool ClientInitializeHUD();
		UUserWidget* GetPrimaryUI() const;
		virtual void PauseMenuEnable(bool bInEnabled);

		template <class T>
		T* GetPrimaryUI() const
		{
			T* retval = Cast<T>(GetPrimaryUI());
			return(retval);
		}

	public:
		virtual void PushExternalWidget(UUserWidget * InWidget);
		virtual bool PopExternalWidget();
		virtual void OnEscapeAction();

		uint32 NumOpenWidgets() const;

		bool IsExternalMenuOpen() const;

		/*Returns True if Any Menu is currently open, Including the Pause Menu*/
		bool IsAnyMenuOpen() const;

	protected:
		virtual bool InitPrimaryUI();
		UUserWidget* GetPauseMenu() const;
		virtual FStackWidgetInfo GetDefaultInputSettings() const;
	
	protected:
		virtual void ProcessInputSettings(const FStackWidgetInfo InWidgetinfo);
		void SetInputMode(const FInputModeDataBase& InData);
		void SetMouseCursorVisible(bool bIsCursorVisible, EMouseCursor::Type CursorType = EMouseCursor::Default);

	protected:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> PauseMenuClass;

		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> PrimaryUIClass;

		UPROPERTY(EditDefaultsOnly)
		FStackWidgetInfo DefaultInputSettings;

	protected:
		UUserWidget * MainUI = nullptr;

		UPROPERTY()
		UUserWidget* PauseMenu = nullptr;

		UPROPERTY()
		TArray<UUserWidget*> ExternalWidgets = TArray<UUserWidget*>();
};
