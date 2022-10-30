// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPropertySlotWidget.h"
#include "..\..\Orders\RTSOrder.h"
#include "..\..\RTSPlayerController.h"

void URTSPropertySlotWidget::Setup(const URTSProperty * InPropertyClass,  TArray<TScriptInterface<IRTSObjectInterface>> InPropertyHolders)
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
	 const URTSOrder * activeprop = Cast<URTSOrder>(BoundProperty);
	if (IsValid(activeprop))
	{
		if (activeprop->RequiresTarget())
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

TArray<TScriptInterface<IRTSObjectInterface>> URTSPropertySlotWidget::GetBestUsersForProperty()
{
	return PropertyHolders;
}

bool URTSPropertySlotWidget::IsQueuedOrder() const
{
	return false;
}
