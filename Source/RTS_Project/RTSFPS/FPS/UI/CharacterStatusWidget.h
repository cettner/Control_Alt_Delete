// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/PanelWidget.h"
#include "CharacterStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UCharacterStatusWidget : public UPanelWidget
{
	GENERATED_BODY()
	
	protected:
		UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
};
