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

	public:
		virtual bool InitializeUI();
		UUserWidget* GetPrimaryUI() const;
		
		template <class T>
		T* GetPrimaryUI() const
		{
			T* retval = Cast<T>(GetPrimaryUI());
			return(retval);
		}

		virtual void SetPrimaryUI(UUserWidget * InUI);

	protected:
		UUserWidget * MainUI = nullptr;
};
