// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "Components/GridPanel.h"
#include "RTSPropertySlotWidget.h"
#include "../RTSSelectionPanelWidget.h"

#include "RTSPropertyPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSPropertyPanelWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UFUNCTION()
	virtual void OnSelectedPropertiesUpdated(const TArray<FSelectionPropertyMap> InSelectionProperties);

public:
	virtual void Setup(RTSSelectionPropertyDelegate & InPropertyDelegate);

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 NumRows = 3;

	UPROPERTY(EditDefaultsOnly)
	int32 NumColumns = 8;

	const int32 MaxSlots = NumRows * NumColumns;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* PropertyGrid = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URTSPropertySlotWidget> PropertySlotClass = nullptr;

protected:

	/*True When the Selection Delegate has been bound */
	bool bIsSelectionDelegatebound = false;
};
