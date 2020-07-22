// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
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
		virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	protected:
		UHealthComponent* GetOwnerHealthComp() const;

	protected:
		UFUNCTION()
		virtual float UpdateHealthPercent() const;

		UFUNCTION()
		virtual FText UpdateCurrentHealthText() const;

		UFUNCTION()
		virtual FText UpdateMaxHealthText() const;


    protected:
		/*Floating Point to Text Formatting Options*/
		FNumberFormattingOptions FloatingPointOptions;

    protected:
	   UPROPERTY(meta = (BindWidget))
	   UProgressBar* HealthBar;

	   UPROPERTY(meta = (BindWidgetOptional))
	   UTextBlock* MaxHealthText;

	   UPROPERTY(meta = (BindWidgetOptional))
	   UTextBlock* CurrentHealthText;
};
