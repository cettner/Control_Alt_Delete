// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Project/RTSFPS/Shared/UI/MaterialProgressBar.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"

#include "Components/TextBlock.h"
#include "FPSExpProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFPSExpProgressBar : public UMaterialProgressBar
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual float UpdatePercentExp();

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock * LevelTextBlock;

	virtual IExpAccumulatorInterface * GetExpUser() const;
};
