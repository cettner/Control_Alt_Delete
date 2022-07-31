// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "RTS_Project/RTSFPS/GameObjects/Resources/Resource.h"
#include "TeamResourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UTeamResourceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Setup(FResourceUIData UIData);


protected:
	UFUNCTION()
	virtual FText UpdateCurrentResourceValueText();

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ResourceNameText;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResourceValueText;


	FResourceUIData SetupData;
};
