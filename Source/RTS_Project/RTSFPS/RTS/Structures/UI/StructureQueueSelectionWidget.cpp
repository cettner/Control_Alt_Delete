// Fill out your copyright notice in the Description page of Project Settings.
#include "StructureQueueSelectionWidget.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"


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

	ARTSPlayerController* pc = GetOwningPlayer<ARTSPlayerController>();
//	pc->ServerPurchaseRTSObject(Structure, BoundQueueData.SpawnClass);
}

bool UStructureQueueSelectionWidget::UpdateSelectionButtonEnabled()
{
	/*If the bound structures Queue is already full or the item isnt unlocked yet*/
	if (Structure->IsQueueFull() || !BoundQueueData.bIsEnabled) return(false);

	bool CanTeamAfford = false;

	UWorld* World = GetWorld();
	if (World == nullptr) return(false);
	
	ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();
	if (GS == nullptr) return(false);

	const ATeamResourceState * ts = GS->GetDefaultTeamState<ATeamResourceState>();

	/*For Each Resource Type needed determine if the team has enough*/
	FReplicationResourceMap cost = FReplicationResourceMap();
	GS->GetUnitPriceForSource(BoundQueueData.SpawnClass, ts, cost, GetOwningPlayer());
	CanTeamAfford = ts->HasResource(cost);

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


