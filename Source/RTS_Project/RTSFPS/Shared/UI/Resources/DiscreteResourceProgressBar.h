// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../MaterialProgressBar.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Resource.h"
#include "DiscreteResourceProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UDiscreteResourceProgressBar : public UMaterialProgressBar
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	virtual float UpdateResourceCapacity();


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResource> TrackedResourceClass = nullptr;
};
