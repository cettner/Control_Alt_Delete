// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Resource.h"
#include "TeamResourceSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UTeamResourceSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void Setup(TSubclassOf<UResource> InResourceClass);

protected:
	UFUNCTION()
	virtual FText UpdateCurrentResourceValueText();

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bIncludeResourceName = true;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResourceValueText;

protected:
	TSubclassOf<UResource> DisplayResourceClass = nullptr;

	FName ResourceName = FName("Error");
};
