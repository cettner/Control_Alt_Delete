// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTSPropertyToolTipWidget.h"
#include "RTSProperty.h"
#include "RTSPropertySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSPropertySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void Setup(const URTSProperty * InProperty, TArray<TScriptInterface<IRTSObjectInterface>> InPropertyHolders);

protected:
	UFUNCTION()
	virtual void OnActivatePropertyClicked();

	virtual TArray<TScriptInterface<IRTSObjectInterface>> GetBestUsersForProperty() const;

	virtual bool IsQueuedOrder() const;
	
	virtual const TArray<TScriptInterface<IRTSObjectInterface>>& GetTotalSelection() const;

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* PropertyImage = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ActivatePropertyButton = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URTSPropertyToolTipWidget> ToolTipClass = URTSPropertyToolTipWidget::StaticClass();

protected:
	const URTSProperty * BoundProperty = nullptr;

	TArray<TScriptInterface<IRTSObjectInterface>> PropertyHolders = TArray<TScriptInterface<IRTSObjectInterface>>();
};
