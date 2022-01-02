// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"

#include "..\Interfaces\UpgradableInterface.h"
#include "..\Interfaces\ExpAccumulatorInterface.h"
#include "UpgradeNodeWidget.h"
#include "UpgradeTreeWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UUpgradeTreeWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		virtual void RefreshNodes();

	protected:
		virtual bool Initialize() override;

	public:
		virtual IUpgradableInterface* GetUpgradeUser() const;
		virtual IExpAccumulatorInterface * GetExpUser() const;

	protected:
		UPROPERTY(meta = (BindWidget))
		UCanvasPanel * NodePanel;

		UPROPERTY(meta = (BindWidgetOptional))
		UTextBlock * UpgradePointTextBlock;

	protected:
		TArray< UUpgradeNodeWidget*> Nodes;

};
