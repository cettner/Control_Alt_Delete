// Fill out your copyright notice in the Description page of Project Settings.
#include "StructureQueueSelectionWidget.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"


void UStructureQueueSelectionWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (Structure == nullptr) return;

	if (MinionNameTextBlock)
	{
		MinionNameTextBlock->SetText(FText::FromString(BoundQueueData.MinionName));
	}
}


void UStructureQueueSelectionWidget::OnSelectionClicked()
{
	if (Structure == nullptr || BoundQueueData.SpawnClass == nullptr)  return;
	
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ARTSPlayerController * PC = World->GetFirstPlayerController<ARTSPlayerController>();
	if (PC == nullptr) return;

	PC->ServerPurchaseMinion(Structure, BoundQueueData.SpawnClass);
}

bool UStructureQueueSelectionWidget::UpdateSelectionButtonEnabled()
{
	/*If the bound structures Queue is already full or the item isnt unlocked yet*/
	if (Structure->IsQueueFull() || !BoundQueueData.bIsEnabled) return(false);

	/*Small Optimization, if there's no costs in the map, its enabled*/
	if (BoundQueueData.ResourceCost.Num() == 0) return(true);

	bool CanTeamAfford = true;

	UWorld* World = GetWorld();
	if (World == nullptr) return(false);
	
	ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();
	ADefaultPlayerController* PC = World->GetFirstPlayerController<ADefaultPlayerController>();
	if (GS == nullptr || PC == nullptr) return(false);

	/*For Each Resource Type needed determine if the team has enough*/
	for (TPair<TSubclassOf<AResource>, int> Elem : BoundQueueData.ResourceCost.GetMap())
	{
		CanTeamAfford &= GS->IsTeamResourceAvailable(PC->GetTeamID(), Elem.Key, Elem.Value);
	}

	return CanTeamAfford;
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
		SelectButton->bIsEnabledDelegate.BindUFunction(this, "UpdateSelectionButtonEnabled");
	}
}


