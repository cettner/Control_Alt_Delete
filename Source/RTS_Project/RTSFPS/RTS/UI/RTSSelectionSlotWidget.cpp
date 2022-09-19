// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSSelectionSlotWidget.h"


void URTSSelectionSlotWidget::Setup(TScriptInterface<IRTSObjectInterface> InObj, const uint8 InNumUnits)
{
	SelectedObjectReference = InObj;
}

void URTSSelectionSlotWidget::SetPrimarySelectionState(const bool InState)
{
	bIsPrimarySelected = InState;
}

bool URTSSelectionSlotWidget::IsPrimarySelection() const
{
	return bIsPrimarySelected;
}
