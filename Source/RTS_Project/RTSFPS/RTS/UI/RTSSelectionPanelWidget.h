// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "Components/GridPanel.h"
#include "RTSSelectionSlotWidget.h"
#include "RTSSelectionPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSSelectionPanelWidget : public UUserWidget
{
	GENERATED_BODY()


	protected:
		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UFUNCTION()
	virtual void OnSelectionUpdated(const TArray<TScriptInterface<IRTSObjectInterface>> InSelectedUnits);

protected:
	UPROPERTY(EditDefaultsOnly)
	uint32 NumRows = 3;

	UPROPERTY(EditDefaultsOnly)
	uint32 NumColumns = 6;

protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* SelectionGrid = nullptr;


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URTSSelectionSlotWidget> SelectionSlotClass = nullptr;

protected:
	bool bIsSelectionDelegatebound = false;


};
