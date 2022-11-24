// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPropertySlotWidget.h"
#include "..\..\Orders\RTSTargetedOrder.h"
#include "..\..\RTSPlayerController.h"
#include "..\..\Camera\RTSSelectionCamera.h"

void URTSPropertySlotWidget::Setup(URTSProperty * InPropertyClass,  TArray<TScriptInterface<IRTSObjectInterface>> InPropertyHolders)
{
	BoundProperty = InPropertyClass;
	PropertyHolders = InPropertyHolders;

	if (IsValid(PropertyImage) && InPropertyClass->GetThumbnail() != nullptr)
	{
		PropertyImage->SetBrushFromTexture(InPropertyClass->GetThumbnail());
	}

	if (IsValid(ActivatePropertyButton))
	{
		ActivatePropertyButton->OnClicked.AddDynamic(this, &URTSPropertySlotWidget::OnActivatePropertyClicked);
	}
}

void URTSPropertySlotWidget::OnActivatePropertyClicked()
{
	 URTSOrder * activeprop = Cast<URTSOrder>(BoundProperty);
	if (IsValid(activeprop))
	{
		if (const URTSTargetedOrder * targetorder = Cast<URTSTargetedOrder>(activeprop))
		{

		}
		else
		{
			ARTSPlayerController * pc = GetOwningPlayer<ARTSPlayerController>();
			const TArray<TScriptInterface<IRTSObjectInterface>> userstoactivate = GetBestUsersForProperty();
			const bool isqueued = IsQueuedOrder();
			pc->IssueOrder(PropertyHolders, activeprop, FHitResult(), isqueued);
		}
	}
}

TArray<TScriptInterface<IRTSObjectInterface>> URTSPropertySlotWidget::GetBestUsersForProperty() const
{
	TArray<TScriptInterface<IRTSObjectInterface>> retval;
	const URTSOrder* activeprop = Cast<URTSOrder>(BoundProperty);
	if (activeprop->UseDefaultOnFail())
	{
		retval = GetTotalSelection();
	}
	else
	{
		retval = activeprop->GetBestMinionsForOrder(PropertyHolders);
	}


	return retval;
}

bool URTSPropertySlotWidget::IsQueuedOrder() const
{
	return false;
}

const TArray<TScriptInterface<IRTSObjectInterface>>& URTSPropertySlotWidget::GetTotalSelection() const
{
	ARTSSelectionCamera * rtscamera = GetOwningPlayerPawn<ARTSSelectionCamera>();

	return rtscamera->GetSelectedUnits();
}
