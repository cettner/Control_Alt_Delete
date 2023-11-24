// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeNodeWidget.h"
#include "../Interfaces/UpgradableInterface.h"
#include "UpgradeTreeWidget.h"


bool UUpgradeNodeWidget::Initialize()
{
	const bool retval = Super::Initialize();
	UpgradeButton->OnClicked.AddDynamic(this, &UUpgradeNodeWidget::OnUpgradeButtonClicked);
	bIsEnabledDelegate.BindDynamic(this, &UUpgradeNodeWidget::IsNodeEnabled);
	ProgressText->TextDelegate.BindDynamic(this, &UUpgradeNodeWidget::GetNodeProgressText);
	return retval;
}

void UUpgradeNodeWidget::AddExternalDependencies(TArray<FUpgradeDependencyInfo>& OutInfo) const
{
}

void UUpgradeNodeWidget::SetProgressText(uint32 current, uint32 max)
{
}

bool UUpgradeNodeWidget::CanPurchaseUpgrade() const
{
	const UUpgradeTreeWidget* parenttree = GetParentTree();
	if (!parenttree) return false;

	const IUpgradableInterface* upgradeuser = parenttree->GetUpgradeUser();
	if (!upgradeuser) return false;

	bool retval = upgradeuser->MeetsUpgradeDependencies(UpgradeToApply);

	return retval;
}

bool UUpgradeNodeWidget::CanRemoveUpgrade() const
{
	const UUpgradeTreeWidget* parenttree = GetParentTree();
	if (!parenttree) return false;

	const IUpgradableInterface* upgradeuser = parenttree->GetUpgradeUser();
	if (!upgradeuser) return false;

	const bool retval = upgradeuser->MeetsRemovalDependencies(UpgradeToApply);

	return retval;
}

void UUpgradeNodeWidget::ApplyUpgrade(IUpgradableInterface* UpgradeUser) const
{

}

void UUpgradeNodeWidget::RemoveUpgrade(IUpgradableInterface* UpgradeUser) const
{

}

FReply UUpgradeNodeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InKeyEvent)
{
	FReply retval = FReply::Unhandled();

	if (InKeyEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnUpgradeButtonRightClicked();
		retval = FReply::Handled();
	}

	return retval;
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

void UUpgradeNodeWidget::OnUpgradeButtonRightClicked()
{
	const UUpgradeTreeWidget* parenttree = GetParentTree();
	IUpgradableInterface* upgradeuser = parenttree->GetUpgradeUser();
	if (CanRemoveUpgrade())
	{
		RemoveUpgrade(upgradeuser);
	}
}

bool UUpgradeNodeWidget::IsNodeEnabled()
{
	return CanPurchaseUpgrade();
}

FText UUpgradeNodeWidget::GetNodeProgressText()
{
	const uint32 maxrank = GetUpgradeMaxRank();
	const uint32 currentrank = GetUserCurrentRank();

	const FString currenttext = FString::FromInt(currentrank);
	const FString maxtext = FString::FromInt(maxrank);

	const FString fullstring = currenttext + " / " + maxtext;
	const FText retval = FText::FromString(fullstring);

	return retval;
}

uint32 UUpgradeNodeWidget::GetUpgradeMaxRank() const
{
	const UUpgrade* defaultupgrade = UpgradeToApply.GetDefaultObject();
	return defaultupgrade->GetMaxRank();

}

uint32 UUpgradeNodeWidget::GetUserCurrentRank() const
{
	uint32 retval = UPGRADE_UNLEARNED;
	const UUpgradeTreeWidget* parenttree = GetParentTree();
	const IUpgradableInterface* upgradeuser = parenttree->GetUpgradeUser();

	if (upgradeuser != nullptr)
	{
		retval = upgradeuser->GetCurrentUpgradeRankFor(UpgradeToApply);
	}

	return retval;
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
	const uint32 currentrank = GetUserCurrentRank();
	const uint32 maxrank = GetUpgradeMaxRank();

	TArray<FUpgradeDependencyInfo> tooltipdependencies = TArray<FUpgradeDependencyInfo>();
	const bool isnodeenabled = UpgradeUser->MeetsUpgradeDependencies(UpgradeToApply, tooltipdependencies);

	UUpgradeToolTipWidget* tooltip = Cast<UUpgradeToolTipWidget>(GetToolTip());
	tooltip->FormatDependencies(tooltipdependencies);

	SetProgressText(currentrank, maxrank);
}

UUpgradeTreeWidget * UUpgradeNodeWidget::GetParentTree() const
{
	return ParentTree;
}
