// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureSpawnQueueWidget.h"


void UStructureSpawnQueueWidget::Setup(ARTSStructure* InStructure)
{
	if (InStructure == nullptr) return;
	
	Structure = InStructure;
	FillSelectionPane();

	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UStructureSpawnQueueWidget::OnCloseButtonClicked);
	}

	if (CurrentStatusBar)
	{
		CurrentStatusBar->PercentDelegate.BindUFunction(this, "UpdateQueueStatusPercent");
	}

	if (MaxQueueSizeTextBlock)
	{
		MaxQueueSizeTextBlock->TextDelegate.BindDynamic(this, &UStructureSpawnQueueWidget::UpdateMaxQueueSizeText);
	}

	if (CurrentQueueSizeTextBlock)
	{
		CurrentQueueSizeTextBlock->TextDelegate.BindDynamic(this, &UStructureSpawnQueueWidget::UpdateCurrentQueueSizeText);
	}
}

void UStructureSpawnQueueWidget::FillSelectionPane()
{
	UWorld* World = this->GetWorld();
	if (SelectionPane == nullptr || Structure == nullptr || World == nullptr || ClassSelectionButtonClass == nullptr) return;

	TArray<FStructureSpawnData> spawndata = Structure->GetSpawnData();
	SelectionPane->ClearChildren();

	for (int i = 0; i < spawndata.Num(); i++)
	{
		if (CanQueueFromMenu(spawndata[i]))
		{
			UStructureQueueSelectionWidget* selection = CreateWidget<UStructureQueueSelectionWidget>(World, ClassSelectionButtonClass);
			if (selection)
			{
				selection->Setup(spawndata[i], Structure);
				SelectionPane->AddChild(selection);
			}
		}
	}
}

void UStructureSpawnQueueWidget::OnCloseButtonClicked()
{
	ARTSPlayerController * PC = GetOwningPlayer<ARTSPlayerController>();
	if (PC)
	{
		PC->CloseExternalMenu();
	}

	RemoveFromParent();
}

bool UStructureSpawnQueueWidget::CanQueueFromMenu(FStructureSpawnData QueueData)
{
	/*Even though Some buildings can spawn Commanders, players shouldn't be able to request it, thats done on death from the game state*/
	if (!QueueData.SpawnClass.Get() || QueueData.SpawnClass.Get()->IsChildOf(ACommander::StaticClass()))
	{
		return(false);
	}
	return true;
}

float UStructureSpawnQueueWidget::UpdateQueueStatusPercent()
{
	float currentstatus = 0.0f;
	if (Structure && Structure->IsValidLowLevel())
	{
		currentstatus = Structure->GetCurrentQueueStatus() / 100.0f;
	}
	return currentstatus;
}

FText UStructureSpawnQueueWidget::UpdateCurrentQueueSizeText()
{
	FText retval = FText::FromString("0");
	
	if (Structure)
	{
		FString stringval = FString::FromInt(Structure->GetCurrentQueueSize());
		retval = FText::FromString(stringval);
	}

	return retval;
}

FText UStructureSpawnQueueWidget::UpdateMaxQueueSizeText()
{
	FText retval = FText::FromString("0");

	if (Structure)
	{
		FString stringval = FString::FromInt(Structure->GetMaxQueueSize());
		retval = FText::FromString(stringval);
	}

	return retval;
}
