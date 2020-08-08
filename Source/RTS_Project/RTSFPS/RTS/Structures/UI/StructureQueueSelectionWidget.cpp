// Fill out your copyright notice in the Description page of Project Settings.


#include "StructureQueueSelectionWidget.h"




void UStructureQueueSelectionWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (Structure == nullptr) return;

	if (Structure->IsQueueFull() || !BoundQueueData.bIsEnabled)
	{
		SelectButton->bIsEnabled = false;
	}
	else
	{
		SelectButton->bIsEnabled = true;
	}

	if (MinionNameTextBlock)
	{
		MinionNameTextBlock->SetText(FText::FromString(BoundQueueData.MinionName));
	}
}


void UStructureQueueSelectionWidget::OnSelectionClicked()
{
	if (Structure == nullptr || BoundQueueData.MinionClass == nullptr)  return;
	Structure->QueueMinion(BoundQueueData.MinionClass);
}

FStructureSpawnData UStructureQueueSelectionWidget::GetBoundData()
{
	return(BoundQueueData);
}

void UStructureQueueSelectionWidget::Setup(FStructureSpawnData QueueData, ARTSStructure* BoundStructure)
{
	BoundQueueData = QueueData;
	Structure = BoundStructure;

	if (SelectButton)
	{
		SelectButton->OnClicked.AddDynamic(this, &UStructureQueueSelectionWidget::OnSelectionClicked);
	}
}


