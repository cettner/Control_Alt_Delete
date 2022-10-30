// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPropertyPanelWidget.h"
#include "Components/GridSlot.h"




void URTSPropertyPanelWidget::OnSelectedPropertiesUpdated(const TArray<FSelectionPropertyMap> InSelectionProperties)
{
	PropertyGrid->ClearChildren();
	
	for (int i = 0; i < InSelectionProperties.Num(); i++)
	{
		URTSPropertySlotWidget* widgetslot = CreateWidget<URTSPropertySlotWidget>(this, PropertySlotClass);
		const URTSProperty * prop = InSelectionProperties[i].Property;
		TArray<TScriptInterface<IRTSObjectInterface>> propowners = InSelectionProperties[i].PropertyOwners;
		widgetslot->Setup(prop, propowners);

		widgetslot->SetPadding(FMargin(4.0f));

		const uint32 colindex = i % NumColumns;
		const uint32 rowindex = i / NumColumns;

		UGridSlot* placedslot = PropertyGrid->AddChildToGrid(widgetslot, rowindex, colindex);
		placedslot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		placedslot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	}
}

void URTSPropertyPanelWidget::Setup(RTSSelectionPropertyDelegate& InPropertyDelegate)
{
	InPropertyDelegate.BindUFunction(this, "OnSelectedPropertiesUpdated");
}
