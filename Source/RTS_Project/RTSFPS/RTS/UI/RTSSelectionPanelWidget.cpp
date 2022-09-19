// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSSelectionPanelWidget.h"
#include "../Camera/RTSSelectionCamera.h"
#include "Components/GridSlot.h"




void URTSSelectionPanelWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsSelectionDelegatebound == false)
	{
		ARTSSelectionCamera* rtscamera = GetOwningPlayerPawn<ARTSSelectionCamera>();
		/*this will fail the first few attempts until the pawn has been replicated down or created*/
		if (IsValid(rtscamera))
		{
			rtscamera->SelectionUpdateDelegate.BindUFunction(this, "OnSelectionUpdated");
			bIsSelectionDelegatebound = true;
		}
	}
}

void URTSSelectionPanelWidget::OnSelectionUpdated(const TArray<TScriptInterface<IRTSObjectInterface>> InSelectedUnits)
{
	SelectionGrid->ClearChildren();
	if (SelectionSlotClass != nullptr)
	{
		if (InSelectedUnits.Num() <= MaxSlots)
		{
			/*If we can draw all the units in available slots do it*/
			DrawSingleUnitPane(InSelectedUnits);
		}
		else
		{
			/*Truncate Minions down to select by class, then draw the number of units selected instead*/
			DrawMultiSelectionPane(InSelectedUnits);
		}
	}
}

void URTSSelectionPanelWidget::DrawSingleUnitPane(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits)
{
	for (int i = 0; i < InSelectedUnits.Num(); i++)
	{
		TScriptInterface<IRTSObjectInterface> rtsobj = InSelectedUnits[i];
		URTSSelectionSlotWidget* widgetslot = CreateWidget<URTSSelectionSlotWidget>(this, SelectionSlotClass);
		widgetslot->Setup(rtsobj);
		widgetslot->SetPadding(FMargin(4.0f));

		const uint32 colindex = i % NumColumns;
		const uint32 rowindex = i / NumColumns;

		UGridSlot* placedslot = SelectionGrid->AddChildToGrid(widgetslot, rowindex, colindex);
		placedslot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		placedslot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}
}

void URTSSelectionPanelWidget::DrawMultiSelectionPane(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits)
{

}

TArray<FSelectionPropertyMap> URTSSelectionPanelWidget::BuildPropertiesFromSelection(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits) const
{
	/*Properties are unique to a class, not an individual unit, so to save time / data flow only get the CDO pointers*/
	TArray<UClass*> objectclasses = TArray<UClass*>();
	for (int i = 0; i < InSelectedUnits.Num(); i++)
	{
		const UObject * rtsobject = InSelectedUnits[i].GetObject();
		checkf(rtsobject, TEXT("URTSSelectionPanelWidget::BuildPropertiesFromSelection Scanned Object was null"));
		objectclasses.AddUnique(rtsobject->GetClass());
	}

	//TODO, Finish this....
	return TArray<FSelectionPropertyMap>();
}
