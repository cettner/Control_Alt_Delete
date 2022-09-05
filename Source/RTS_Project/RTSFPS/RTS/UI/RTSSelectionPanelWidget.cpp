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
		const uint32 maxslots = NumColumns * NumRows;

		for (size_t i = 0; i < maxslots; i++)
		{
			URTSSelectionSlotWidget * widgetslot = CreateWidget<URTSSelectionSlotWidget>(this, SelectionSlotClass);
			const uint32 colindex = i % NumColumns;
			const uint32 rowindex = i / NumColumns;
			UGridSlot * placedslot = SelectionGrid->AddChildToGrid(widgetslot, rowindex, colindex);
			placedslot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			placedslot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

			/*Note: this is the padding for widget, not the slot, they are in fact different*/
			widgetslot->SetPadding(FMargin(4.0f));
		}
	}
}
