// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "TabButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UTabButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetActive(bool InActiveState);

	UFUNCTION()
	FSlateColor GetTitleColor();

	virtual UButton* GetButton();

	virtual void SetBoundWidget(UWidget * InWidget);
	UWidget* GetBoundWidget() const;
	bool IsWidgetBound() const;
	


protected:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* InnerButton = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TabNameTextBlock = nullptr;


protected:
	UPROPERTY()
	FLinearColor ActiveTextColor = FLinearColor();

	UPROPERTY()
	FLinearColor InActiveTextColor = FLinearColor();


protected:
	bool bIsActiveTab = false;

	UWidget* BoundWidget = nullptr;
	
};
