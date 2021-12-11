// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"

#include "..\Upgrade.h"
#include "UpgradeNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UUpgradeNodeWidget : public UUserWidget
{
	GENERATED_BODY()
	

	protected:


	protected:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUpgrade> UpgradeToApply;

		IUpgradableInterface * UpgradeUser;
};
