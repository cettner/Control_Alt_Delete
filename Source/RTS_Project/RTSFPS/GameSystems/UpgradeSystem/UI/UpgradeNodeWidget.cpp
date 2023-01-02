// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeNodeWidget.h"
#include "../Interfaces/UpgradableInterface.h"
#include "UpgradeTreeWidget.h"


bool UUpgradeNodeWidget::Initialize()
{
	const bool retval = Super::Initialize();
	UpgradeButton->OnClicked.AddDynamic(this, &UUpgradeNodeWidget::OnUpgradeButtonClicked);

	return retval;
}

void UUpgradeNodeWidget::AddExternalDependencies(TArray<FUpgradeDependencyInfo>& OutInfo) const
{
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
	SetIsEnabled(isenabled);
}

bool UUpgradeNodeWidget::CanPurchaseUpgrade() const
{
	const uint32 maxrank = GetUpgradeMaxRank();
	const bool isenabled = GetIsEnabled();
	return isenabled && (CurrentRank < maxrank);
}

void UUpgradeNodeWidget::ApplyUpgrade(IUpgradableInterface* UpgradeUser) const
{

}

void UUpgradeNodeWidget::OnUpgradeButtonClicked()
{
	const UUpgradeTreeWidget * parenttree = GetParentTree();
	IUpgradableInterface* upgradeuser = parenttree->GetUpgradeUser();
	if (CanPurchaseUpgrade()  && (upgradeuser != nullptr))
	{
		ApplyUpgrade(upgradeuser);
	}
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

bool UUpgradeNodeWidget::Setup(UUpgradeTreeWidget * InParentTree)
{
	checkf(InParentTree, TEXT("UUpgradeNodeWidget::Setup InvalidParent Tree"))
	ParentTree = InParentTree;

	UUpgradeToolTipWidget* tooltip = CreateWidget<UUpgradeToolTipWidget>(this, ToolTipClass);
	tooltip->Setup(UpgradeToApply);
	SetToolTip(tooltip);

	return true;
}

void UUpgradeNodeWidget::RefreshNode(const IUpgradableInterface* UpgradeUser)
{
	CurrentRank = UpgradeUser->GetCurrentUpgradeRankFor(UpgradeToApply);
	const uint32 maxrank = GetUpgradeMaxRank();
	const UUpgrade* defaultupgrade = UpgradeToApply.GetDefaultObject();


	TArray<FUpgradeDependencyInfo> tooltipdependencies;
	const bool isnodeenabled = defaultupgrade->CanUpgrade(UpgradeUser, tooltipdependencies);

	UUpgradeToolTipWidget* tooltip = Cast<UUpgradeToolTipWidget>(GetToolTip());
	tooltip->FormatDependencies(tooltipdependencies);


	SetProgressText(CurrentRank, maxrank);
	SetNodeEnabled(isnodeenabled);
}

UUpgradeTreeWidget * UUpgradeNodeWidget::GetParentTree() const
{
	return ParentTree;
}
