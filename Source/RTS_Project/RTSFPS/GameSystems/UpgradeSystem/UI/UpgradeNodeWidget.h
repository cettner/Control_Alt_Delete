// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Containers/Map.h"

#include "..\Upgrade.h"
#include "UpgradeNodeWidget.generated.h"

/**
 * 
 */



UCLASS()
class RTS_PROJECT_API UUpgradeNodeWidget : public UUserWidget
{
	GENERATED_BODY()


	public:
		virtual void RefreshNode(const IUpgradableInterface * UpgradeUser);

	protected:
		virtual bool IsNodeEnabled() const;
		virtual bool ShouldShouldEnableAbility() const;
		virtual void SetProgressText(uint32 current, uint32 max);
		virtual void SetNodeEnabled(bool isenabled);

		uint32 GetUpgradeMaxRank() const;
		TArray<FUpgradeUnlockCondition> GetUnlockConditions() const;
		TArray<TSubclassOf<UUpgrade>> GetExclusiveConditions() const;


	protected:
		virtual bool Setup(IUpgradableInterface * UpgradeUser);


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


	protected:
		uint32 CurrentRank = 0;
};
