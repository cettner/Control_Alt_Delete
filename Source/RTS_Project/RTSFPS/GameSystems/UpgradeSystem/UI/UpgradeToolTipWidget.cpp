// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeToolTipWidget.h"

void UUpgradeToolTipWidget::FormatDependencies(TArray<FUpgradeDependencyInfo> InUpgradeDependencies)
{
	DependencyPanelWidget->ClearChildren();

	for (int i = 0; i < InUpgradeDependencies.Num(); i++)
	{
		const FString descriptionstring = InUpgradeDependencies[i].Description;
		UTextBlock * dependencyblock = NewObject<UTextBlock>(this, UTextBlock::StaticClass());

		FSlateColor color;

		if (InUpgradeDependencies[i].IsSatisfied == true)
		{
			color = FSlateColor(FLinearColor::Green);
		}
		else
		{
			color = FSlateColor(FLinearColor::Red);
		}

		dependencyblock->ColorAndOpacity = color;
		dependencyblock->SetText(FText::FromString(descriptionstring));

		DependencyPanelWidget->AddChild(dependencyblock);
	}
}

void UUpgradeToolTipWidget::Setup(TSubclassOf<UUpgrade> InUpgradeClass)
{
	const UUpgrade* owningupgrade = InUpgradeClass.GetDefaultObject();
	const FName upgradename = owningupgrade->GetUpgradeName();
	const FText nametext = FText::FromName(upgradename);


	UpgradeNameTextBlock->SetText(nametext);
}