// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "StructureQueueSelectionWidget.h"
#include "StructureSpawnQueueWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UStructureSpawnQueueWidget : public UUserWidget
{
	GENERATED_BODY()
	
    public:
        virtual void Setup(ARTSStructure * InStructure);

    protected:
        virtual void FillSelectionPane();

        UFUNCTION()
        virtual void OnCloseButtonClicked();

        virtual bool CanQueueFromMenu(FStructureSpawnData QueueData);

        UFUNCTION()
        float UpdateQueueStatusPercent();

        UFUNCTION()
        FText UpdateCurrentQueueSizeText();

        UFUNCTION()
        FText UpdateMaxQueueSizeText();

    protected:
        ARTSStructure * Structure;

        UPROPERTY(EditDefaultsOnly)
        TSubclassOf<UStructureQueueSelectionWidget> ClassSelectionButtonClass;

    protected:
        UPROPERTY(meta = (BindWidget))
        UProgressBar * CurrentStatusBar;

        UPROPERTY(meta = (BindWidget))
        UPanelWidget * CurrentQueue;

        UPROPERTY(meta = (BindWidget))
        UPanelWidget * SelectionPane;

        UPROPERTY(meta = (BindWidget))
        UButton* CloseButton;

        UPROPERTY(meta = (BindWidgetOptional))
        UTextBlock* CurrentQueueSizeTextBlock;

        UPROPERTY(meta = (BindWidgetOptional))
        UTextBlock* MaxQueueSizeTextBlock;
};
