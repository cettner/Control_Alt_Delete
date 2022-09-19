// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTSSelectionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSSelectionSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Setup(TScriptInterface<IRTSObjectInterface> InObj,const uint8 InNumUnits = 1U);
	virtual void SetPrimarySelectionState(const bool InState);
	virtual bool IsPrimarySelection() const;

protected:
	TScriptInterface<IRTSObjectInterface> SelectedObjectReference = nullptr;
	bool bIsPrimarySelected = false;
};
