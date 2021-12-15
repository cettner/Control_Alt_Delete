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
		virtual bool ShouldShowProgress() const;
		virtual void SetProgressText(uint32 current, uint32 max);

	protected:
		virtual bool Initialize() override;


		/*Configuration Data*/
	protected:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUpgrade> UpgradeToApply = nullptr;

		//UPROPERTY(EditDefaultsOnly)
		//TArray<TPair<int,int>> ParentUpgradeConditions;

		/*Widget Design Data*/
	protected:
		UPROPERTY(meta = (BindWidget))
		UButton* SelectButton;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* ProgressText;

		/*Runtime Data*/
	protected:
		uint32 CurrentRank = 0;

		uint32 MaxRank = 0;
};
