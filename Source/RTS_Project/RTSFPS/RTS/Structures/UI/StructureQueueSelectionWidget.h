// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "StructureQueueSelectionWidget.generated.h"


UCLASS()
class RTS_PROJECT_API UStructureQueueSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

    public:
		FStructureSpawnData GetBoundData();
		void Setup(FStructureSpawnData QueueData, ARTSStructure * BoundStructure);

   protected:
	   virtual void SynchronizeProperties() override;

   protected:
	   UFUNCTION()
	   virtual void OnSelectionClicked();

	   UFUNCTION()
	   virtual bool UpdateSelectionButtonEnabled();

    protected:
		UPROPERTY(meta = (BindWidget))
		UButton* SelectButton;

		UPROPERTY(meta = (BindWidgetOptional))
		UTextBlock* MinionNameTextBlock;

		ARTSStructure* Structure;

		FStructureSpawnData BoundQueueData;
};
