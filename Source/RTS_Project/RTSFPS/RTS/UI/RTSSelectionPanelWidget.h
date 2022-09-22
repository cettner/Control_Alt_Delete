// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "Components/GridPanel.h"
#include "RTSSelectionSlotWidget.h"
#include "Properties/RTSProperty.h"
#include "RTSSelectionPanelWidget.generated.h"


USTRUCT()
struct FSelectionPropertyMap
{
	GENERATED_USTRUCT_BODY()

	TSubclassOf<URTSProperty> Property = nullptr;

	TArray<TScriptInterface<IRTSObjectInterface>> PropertyOwners = TArray<TScriptInterface<IRTSObjectInterface>>();
};

/*Delegate to broadcast what properties we want to show for the selected unit(s)*/
DECLARE_DELEGATE_OneParam(RTSSelectionPropertyDelegate, const TArray<FSelectionPropertyMap>);

UCLASS()
class RTS_PROJECT_API URTSSelectionPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UFUNCTION()
	virtual void OnSelectionUpdated(const TArray<TScriptInterface<IRTSObjectInterface>> InSelectedUnits);
	virtual void DrawSingleUnitPane(const TArray<TScriptInterface<IRTSObjectInterface>> & InSelectedUnits);
	virtual void DrawMultiSelectionPane(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits);

protected:
	virtual const TArray<TScriptInterface<IRTSObjectInterface>> GetRequestedPropertyUnits(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits);
	static TArray<FSelectionPropertyMap> BuildPropertiesFromSelection(const TArray<TScriptInterface<IRTSObjectInterface>>& InSelectedUnits);

public:
	/*Broadcast the requested Properties from the selection Pane, this is done here instead of multicast to the properties panel 
	because the primary selection option on the UI is only known here, as is the associated unit class for a given property*/
	RTSSelectionPropertyDelegate SelectedPropertiesDelegate = RTSSelectionPropertyDelegate();

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 NumRows = 3;

	UPROPERTY(EditDefaultsOnly)
	int32 NumColumns = 8;

	const int32 MaxSlots = NumRows * NumColumns;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* SelectionGrid = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URTSSelectionSlotWidget> SelectionSlotClass = nullptr;

protected:
	/**/
	URTSSelectionSlotWidget* PrimarySelectedSlot = nullptr;

	/*True When the Selection Delegate has been bound */
	bool bIsSelectionDelegatebound = false;
};
