// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "FPSUI.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFPSUI : public UUserWidget
{
	GENERATED_BODY()

    public:
		UFPSUI(const FObjectInitializer& ObjectInitializer);

	protected:
		virtual bool Initialize() override;

	protected:
		UFUNCTION()
		virtual void UpdateHealthPercent();

    protected:
		/*Pointer to Owners Pawn for ease of access*/
		APawn* OwnerPawn;

    protected:
	   UPROPERTY(meta = (BindWidget))
	   UProgressBar* HealthBar;

};
