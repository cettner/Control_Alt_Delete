// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeNodeWidget.h"
#include "../Interfaces/UpgradableInterface.h"

bool UUpgradeNodeWidget::IsNodeEnabled() const
{
	return false;
}

bool UUpgradeNodeWidget::ShouldShouldEnableAbility() const
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

void UUpgradeNodeWidget::SetNodeEnabled(bool isenabled)
{
	
}

uint32 UUpgradeNodeWidget::GetUpgradeMaxRank() const
{
	const UUpgrade* defaultupgrade = UpgradeToApply.GetDefaultObject();
	return defaultupgrade->GetMaxRank();

}

TArray<FUpgradeUnlockCondition> UUpgradeNodeWidget::GetUnlockConditions() const
{
	const UUpgrade* defaultupgrade = UpgradeToApply.GetDefaultObject();
	return defaultupgrade->GetUnlockConditions();
}

TArray<TSubclassOf<UUpgrade>> UUpgradeNodeWidget::GetExclusiveConditions() const
{
	const UUpgrade* defaultupgrade = UpgradeToApply.GetDefaultObject();
	return defaultupgrade->GetExclusiveConditions();
}

bool UUpgradeNodeWidget::Setup(IUpgradableInterface* UpgradeUser)
{

	uint32 currentdisplayrank = 0U;
	currentdisplayrank = UpgradeUser->GetCurrentUpgradeTierFor(UpgradeToApply);
		
	const uint32 maxrank = GetUpgradeMaxRank();
	const TArray<TSubclassOf<UUpgrade>> exclusions = GetExclusiveConditions();
	const TArray<FUpgradeUnlockCondition> unlockconditions = GetUnlockConditions();
		
	bool isnodeenabled = true;

	for (int i = 0; i < unlockconditions.Num(); i++)
	{
		const TSubclassOf<UUpgrade> unlockparent = unlockconditions[i].GetParent();
		const uint32 unlockrank = unlockconditions[i].GetRank();

		const uint32 currentrank =  UpgradeUser->GetCurrentUpgradeTierFor(unlockparent);

		if (currentrank < unlockrank)
		{
			isnodeenabled = false;
		}

	}

	for (int i = 0; i < exclusions.Num(); i++)
	{
		const uint32 currentrank = UpgradeUser->GetCurrentUpgradeTierFor(exclusions[i]);
		if (currentrank > UPGRADE_UNLEARNED)
		{
			isnodeenabled = false;
		}
	}


	SetProgressText(currentdisplayrank, maxrank);


	return true;
}

void UUpgradeNodeWidget::RefreshNode(const IUpgradableInterface* UpgradeUser)
{

}