// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Interfaces/ExternalWidgetInterface.h"
#include "ExternalWidget.generated.h"
/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UExternalWidget : public UUserWidget, public IExternalWidgetInterface
{
	GENERATED_BODY()

	public:
		virtual FStackWidgetInfo GetWidgetInputSettings() const override;
		virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	protected:
		virtual void OnEscapeAction();

	protected:
		UPROPERTY(EditDefaultsOnly)
		FStackWidgetInfo DefaultInputSettings = FStackWidgetInfo();

};
