// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeTreeWidget.h"

#include "GameFramework/PlayerState.h"





bool UUpgradeTreeWidget::Initialize()
{
	bool retval = Super::Initialize();

	if(NodePanel != nullptr)
	{
		const TArray<UWidget* > childwidgets = NodePanel->GetAllChildren();
		for (int i = 0; i < childwidgets.Num(); i++)
		{
			UUpgradeNodeWidget* node = Cast<UUpgradeNodeWidget>(childwidgets[i]);
			if (IsValid(node))
			{
				Nodes.Emplace(node);
				node->Setup(this);
			}
		}
	}
	else
	{
		retval = false;
	}

	return retval;
}

void UUpgradeTreeWidget::RefreshNodes()
{
	const IUpgradableInterface * upgradeuser = GetUpgradeUser();
	checkf(upgradeuser,TEXT("UUpgradeTreeWidget::RefreshNodes failed to obtain UpgradeUser"))

	for (int i = 0; i < Nodes.Num(); i++)
	{
		Nodes[i]->RefreshNode(upgradeuser);
	}

	const IExpAccumulatorInterface * expuser = GetExpUser();
	if (UpgradePointTextBlock != nullptr)
	{
		const FString preamble =  "Points Available: ";
		const FString numpointsavailable = FString::FromInt(expuser->GetAvailableUpgradePoints());

		FText endtext = FText::FromString(preamble + numpointsavailable);
		UpgradePointTextBlock->SetText(endtext);
	}
}

IUpgradableInterface* UUpgradeTreeWidget::GetUpgradeUser() const
{
	IUpgradableInterface* retval = nullptr;
	if (GetOwningPlayer())
	{
		retval = GetOwningPlayer()->GetPlayerState<IUpgradableInterface>();
	}

	return retval;
}

IExpAccumulatorInterface * UUpgradeTreeWidget::GetExpUser() const
{
	IExpAccumulatorInterface * retval = nullptr;
	if (GetOwningPlayer())
	{
		retval = GetOwningPlayer()->GetPlayerState<IExpAccumulatorInterface>();
	}

	return retval;
}
