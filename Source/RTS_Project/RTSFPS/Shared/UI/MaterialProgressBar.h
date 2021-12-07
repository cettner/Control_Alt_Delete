// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Image.h"
#include "MaterialProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMaterialProgressBar : public UUserWidget
{
	GENERATED_BODY()
	

	public:
		UFUNCTION()
		void SetPercent(float InPercent);

	protected:
		virtual void SynchronizeProperties() override;
		virtual void NativeConstruct() override;
		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	protected:
		virtual UMaterialInstanceDynamic * GetMaterialInterface() const;

	protected:
		UPROPERTY(meta = (BindWidget))
		UImage * ProgressBarImage;

		/** A bindable delegate to allow logic to drive the text of the widget */
		UPROPERTY()
		FGetFloat PercentDelegate;

		UPROPERTY(EditDefaultsOnly)
		float LerpDuration = 4.00f;

	private:

		float Percent = 0.0;

		float LastStablePercent = 0.0;

		float LerpPercent = 0.0;



};
