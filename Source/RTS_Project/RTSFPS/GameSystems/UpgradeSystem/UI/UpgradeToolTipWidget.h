// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "..\Upgrade.h"
#include "UpgradeToolTipWidget.generated.h"






UCLASS()
class RTS_PROJECT_API UUpgradeToolTipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Setup(TSubclassOf<UUpgrade> InUpgradeClass);
	virtual void FormatDependencies(TArray<FUpgradeDependencyInfo> InUpgradeDependencies);

protected:
	virtual void FormatDependencyText(const FUpgradeDependencyInfo InInfo, UTextBlock * DependencyBlock);

protected:
	/*****************Widget Design Data**********************/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* UpgradeNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* UpgradeDescriptionTextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UPanelWidget* DependencyPanelWidget;
	/**************************************************/


protected:
	UPROPERTY(EditDefaultsOnly)
	FSlateFontInfo DependencyFont;


};
