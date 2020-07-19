// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "RTS_Project/RTSFPS/FPS/UI/FPSUI.h"
#include "RTS_Project/RTSFPS/RTS/UI/RTSUI.h"
#include "RTSFPSWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSFPSWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		URTSFPSWidget(const FObjectInitializer & ObjectInitializer);
		
    public:

		virtual void Setup();
		virtual void OnHUDStateChange();


	protected:
		UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* RTSFPSUISwitcher;
	
		UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> FPSUIClass;
		UFPSUI* FPSUI;

		UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> RTSUIClass;
		URTSUI* RTSUI;

};
