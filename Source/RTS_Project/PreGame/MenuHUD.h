// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"

#include "MenuHUD.generated.h"




UCLASS()
class RTS_PROJECT_API AMenuHUD : public AHUD
{
	GENERATED_BODY()

	public:
		UWidget* GetCurrentMenu()  const { return CurrentMenu; }

		template<class T>
		T* GetCurrentMenu() const { return Cast<T>(GetCurrentMenu()); }
	
	protected:
		virtual void InitializeMenu();

	protected:
		virtual void PostInitializeComponents() override;


	protected:
		UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> MenuClass;

	protected:
		UWidget* CurrentMenu;

};
