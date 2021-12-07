// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/UI/MaterialProgressBar.h"
#include "PersonalResourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UPersonalResourceWidget : public UMaterialProgressBar
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual float UpdateWeightCapacity();
};
