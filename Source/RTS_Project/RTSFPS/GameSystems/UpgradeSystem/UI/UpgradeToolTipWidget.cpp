// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeToolTipWidget.h"

void UUpgradeToolTipWidget::FormatDependencies(TArray<FUpgradeDependencyInfo> InUpgradeDependencies)
{
	DependencyPanelWidget->ClearChildren();

	for (int i = 0; i < InUpgradeDependencies.Num(); i++)
	{
		const FString descriptionstring = InUpgradeDependencies[i].Description;
		UTextBlock * dependencyblock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());
		FormatDependencyText(InUpgradeDependencies[i], dependencyblock);
		dependencyblock->SetText(FText::FromString(descriptionstring));

		DependencyPanelWidget->AddChild(dependencyblock);
	}
}

void UUpgradeToolTipWidget::FormatDependencyText(const FUpgradeDependencyInfo InInfo, UTextBlock * DependencyBlock) const
{
	FSlateColor color;


	if (InInfo.IsSatisfied == true)
	{
		color = FSlateColor(FLinearColor::Green);
	}
	else
	{
		color = FSlateColor(FLinearColor::Red);
	}

	DependencyBlock->SetColorAndOpacity(color);
	DependencyBlock->SetFont(DependencyFont);
	DependencyBlock->SetAutoWrapText(true);
}

void UUpgradeToolTipWidget::Setup(TSubclassOf<UUpgrade> InUpgradeClass)
{
	const UUpgrade* owningupgrade = InUpgradeClass.GetDefaultObject();
	
	#if WITH_EDITOR
	/*Nodes are Blank originally when added in editor*/
	if (owningupgrade == nullptr) return;
	#endif

	const FName upgradename = owningupgrade->GetUpgradeName();
	const FText nametext = FText::FromName(upgradename);

	const FString upgradedescription = owningupgrade->GetUpgradeDescription();
	const FText descriptiontext = FText::FromString(upgradedescription);

	UpgradeNameTextBlock->SetText(nametext);
	UpgradeDescriptionTextBlock->SetText(descriptiontext);
}