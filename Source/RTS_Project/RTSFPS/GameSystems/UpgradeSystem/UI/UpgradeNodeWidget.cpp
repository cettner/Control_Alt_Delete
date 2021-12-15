// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeNodeWidget.h"

bool UUpgradeNodeWidget::IsNodeEnabled() const
{
	return false;
}

bool UUpgradeNodeWidget::ShouldShowProgress() const
{
	return true;
}

void UUpgradeNodeWidget::SetProgressText(uint32 current, uint32 max)
{
	const FString currenttext = FString::FromInt(current);
	const FString maxtext = FString::FromInt(max);
	
	const FString fullstring = currenttext + " / " + maxtext;
	const FText ptext = FText::FromString(fullstring);

	ProgressText->SetText(ptext);
}

bool UUpgradeNodeWidget::Initialize()
{
	bool retval = Super::Initialize();
	if (UpgradeToApply != nullptr)
	{
		const UUpgrade* defaultupgrade = UpgradeToApply.GetDefaultObject();
		MaxRank = defaultupgrade->GetMaxRank();
		SetProgressText(CurrentRank, MaxRank);
	}

	return true;
}

void UUpgradeNodeWidget::RefreshNode(const IUpgradableInterface* UpgradeUser)
{

}