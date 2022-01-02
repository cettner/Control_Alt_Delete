// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"

#include "Components/TextBlock.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "RTS_Project/RTSFPS/Shared/UI/TeamResourceWidget.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UI/UpgradeTreeWidget.h"
#include "FPSUI.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFPSUI : public UUserWidget
{
	GENERATED_BODY()

    public:
		virtual void ShouldShowUpgradeTree(bool bShowTree);
		virtual void RefreshUpgradeTree();

	protected:
		UFPSUI(const FObjectInitializer& ObjectInitializer);
		virtual bool Initialize() override;

    protected:
	   UPROPERTY(EditDefaultsOnly)
	   TSubclassOf<UTeamResourceWidget> ResourceWidgetClass;

    protected:
	   UPROPERTY(meta = (BindWidgetOptional))
	   UPanelWidget * TeamResourceList;

	   UPROPERTY(meta = (BindWidgetOptional))
	   UWidgetSwitcher * FPSTabSwitcher;

	   UPROPERTY(meta = (BindWidgetOptional))
	   UWidget * MainFPSUI;
	   
	   UPROPERTY(meta = (BindWidgetOptional))
	   UUpgradeTreeWidget * UpgradeTreeWidget;
	   
};
