// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DefaultGameState.h"
#include "DefaultHUD.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()

	protected:
		virtual void PostInitializeComponents() override;

	public:
		virtual bool InitializeUI();
		UUserWidget* GetPrimaryUI() const;

		virtual void PauseMenuEnable(bool bInEnabled);

		template <class T>
		T* GetPrimaryUI() const
		{
			T* retval = Cast<T>(GetPrimaryUI());
			return(retval);
		}

	protected:
		virtual bool InitPrimaryUI();
		UUserWidget* GetPauseMenu() const;

	protected:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> PauseMenuClass;

		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> PrimaryUIClass;

	protected:
		UUserWidget * MainUI = nullptr;

		UUserWidget* PauseMenu = nullptr;
};
