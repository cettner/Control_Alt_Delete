// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"

#include "..\Interfaces\UpgradableInterface.h"
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

	protected:

		virtual IUpgradableInterface* GetUpgradeUser() const;

	protected:
		UPROPERTY(meta = (BindWidget))
		UCanvasPanel * NodePanel;

	protected:
		TArray< UUpgradeNodeWidget*> Nodes;

};
