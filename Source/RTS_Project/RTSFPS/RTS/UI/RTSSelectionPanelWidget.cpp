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

	const TArray<TScriptInterface<IRTSObjectInterface>> groomedselection = GetRequestedPropertyUnits(InSelectedUnits);
	const TArray<FSelectionPropertyMap> propertymap = BuildPropertiesFromSelection(groomedselection);

	
	SelectedPropertiesDelegate.ExecuteIfBound(propertymap);

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

const TArray<TScriptInterface<IRTSObjectInterface>> URTSSelectionPanelWidget::GetRequestedPropertyUnits(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits)
{
	return InSelectedUnits;
}

TArray<FSelectionPropertyMap> URTSSelectionPanelWidget::BuildPropertiesFromSelection(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits)
{
	TArray<FSelectionPropertyMap> retval = TArray<FSelectionPropertyMap>();
	TArray<IRTSObjectInterface*> classcdos = TArray<IRTSObjectInterface*>();
	TArray<IRTSObjectInterface*> classinstances = TArray<IRTSObjectInterface*>();
	TArray<const URTSProperty*> allproperties = TArray<const URTSProperty*>();
	/*Anthony Will fix this*/

	/*Get All Unique CDO's for the selection, this saves time as the number of units in*/
	for (int i = 0; i < InSelectedUnits.Num(); i++)
	{
		const UObject* rtsobj = InSelectedUnits[i].GetObject();
		checkf(rtsobj, TEXT("URTSSelectionPanelWidget::BuildPropertiesFromSelection Interface Object was Null"));
		IRTSObjectInterface* objCDO = CastChecked<IRTSObjectInterface>(rtsobj->GetClass()->GetDefaultObject());
		int32 uniqueindex = classcdos.AddUnique(objCDO);

		if (uniqueindex != INDEX_NONE)
		{
			classinstances.Emplace(CastChecked<IRTSObjectInterface>(InSelectedUnits[i].GetObject()));
		}
	}

	for (int i = 0; i < classinstances.Num(); i++)
	{
		/*Get all properties for one instance of the class*/
		TArray<URTSProperty*> classproperties = classinstances[i]->GetRTSProperties();
			
		/*map each of those properties to all instances that contain it*/
		for (int k = 0; k < classproperties.Num(); k++)
		{
			int32 isunique = allproperties.AddUnique(classproperties[k]);
			if (isunique > INDEX_NONE)
			{
				FSelectionPropertyMap propinsert = FSelectionPropertyMap();
				propinsert.Property = classproperties[k];
				for (int h = 0; h < InSelectedUnits.Num(); h++)
				{
					if (InSelectedUnits[h]->ContainsProperty(propinsert.Property))
					{
						propinsert.PropertyOwners.AddUnique(InSelectedUnits[h]);
					}
				}
				retval.Emplace(propinsert);
			}
		}
	}

	return retval;
}
