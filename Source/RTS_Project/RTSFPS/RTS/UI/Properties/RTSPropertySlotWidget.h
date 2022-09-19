// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	virtual void Setup(const TSubclassOf<URTSProperty> InPropertyClass, TArray<TScriptInterface<IRTSObjectInterface>> InPropertyHolders);


protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* PropertyImage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URTSPropertyToolTipWidget> ToolTipClass = URTSPropertyToolTipWidget::StaticClass();
};
