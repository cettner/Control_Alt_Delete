// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Containers/Map.h"

#include "..\Upgrade.h"
#include "UpgradeNodeWidget.generated.h"

/**
 * 
 */

/*Foward Declaration*/
class UUpgradeTreeWidget;


UCLASS()
class RTS_PROJECT_API UUpgradeNodeWidget : public UUserWidget
{
	GENERATED_BODY()
		

	protected:
		virtual void NativeConstruct() override;

	public:
		virtual bool Setup(UUpgradeTreeWidget* InParentTree);
		virtual void RefreshNode(const IUpgradableInterface * UpgradeUser);
		UUpgradeTreeWidget* GetParentTree() const;

	protected:
		virtual void SetProgressText(uint32 current, uint32 max);
		
		virtual void SetNodeEnabled(bool isenabled);

		/*Whether the User has enough resource / talent points to purchase */
		virtual bool CanPurchaseUpgrade() const;

		virtual void ApplyUpgrade(IUpgradableInterface* UpgradeUser) const;
		
		UFUNCTION()
		virtual void OnUpgradeButtonClicked();

		uint32 GetUpgradeMaxRank() const;
		TArray<FUpgradeUnlockCondition> GetUnlockConditions() const;
		TArray<TSubclassOf<UUpgrade>> GetExclusiveConditions() const;


		/*Configuration Data*/
	protected:
		UPROPERTY(EditAnywhere)
		TSubclassOf<UUpgrade> UpgradeToApply = nullptr;

		/*Widget Design Data*/
	protected:
		UPROPERTY(meta = (BindWidget))
		UButton* UpgradeButton;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* ProgressText;

		UPROPERTY(meta = (BindWidgetOptional))
		UImage * ButtonImage;

		/*Runtime Data*/
	protected:
		UUpgradeTreeWidget* ParentTree = nullptr;
		
		uint32 CurrentRank = 0U;

};
