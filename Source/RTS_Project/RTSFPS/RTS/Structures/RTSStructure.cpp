// Fill out your copyright notice in the Description page of Project Settings.=
#include "RTSStructure.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"
#include "UI/StructureSpawnQueueWidget.h"

#include "Components/DecalComponent.h"
#include "Materials/Material.h"

// Sets default values
ARTSStructure::ARTSStructure(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Selection = CreateDefaultSubobject<URTSSelectionComponent>(TEXT("SelectionComp"));

	USkeletalMeshComponent * Mesh = GetSkeletalMeshComponent();
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCanEverAffectNavigation(true);
		Mesh->bFillCollisionUnderneathForNavmesh = true;
		Selection->SetDetection(Mesh);
		Selection->SetRoot(Mesh);
	}


	MenuClass = nullptr;
	Menu = nullptr;
}

UUserWidget* ARTSStructure::GetMenu()
{
	UWorld* World = GetWorld();

	if (World == nullptr) return nullptr;

	if (MenuClass)
	{
		Menu = CreateWidget <UStructureSpawnQueueWidget>(World, MenuClass);
		if (Menu)
		{
			Menu->Setup(this);
		}
	}

	return(Menu);
}

bool ARTSStructure::CanOpenMenu(APawn* InvokingPawn) const
{
	ARTSMinion * minion = Cast<ARTSMinion>(InvokingPawn);
	if (minion)
	{
		return(GetTeam() == minion->GetTeam());
	}
	return false;
}

void ARTSStructure::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ARTSStructure::SetSelected()
{
}

void ARTSStructure::SetDeselected()
{
}

int ARTSStructure::GetTeam() const
{
	return teamindex;
}

void ARTSStructure::SetTeam(int newteamindex)
{
	teamindex = newteamindex;
}

bool ARTSStructure::IsDropPointFor(TSubclassOf<AResource> ResourceType) const
{
	return (true);
}

bool ARTSStructure::IsQueueFull() const
{
	return (CurrentQueueSize >= MaxQueueSize);
}

bool ARTSStructure::CanSpawn(TSubclassOf<ARTSMinion> minionclass) const
{
	return (GetIndexByClass(minionclass) > -1);
}

int ARTSStructure::GetIndexByClass(TSubclassOf<ARTSMinion> minionclass) const
{
	int index = -1;
	for (int i = 0; i < SpawnableUnits.Num(); i++)
	{
		TSubclassOf<ARTSMinion> availableclasses = SpawnableUnits[i].MinionClass;

		if (availableclasses.Get()->IsChildOf(minionclass.Get()))
		{
			index = i;
			break;
		}
	}
	return index;
}

TArray<FStructureSpawnData> ARTSStructure::GetSpawnData() const
{
	return SpawnableUnits;
}

float ARTSStructure::GetCurrentQueueStatus() const
{
	return queuestatus;
}

uint32 ARTSStructure::GetCurrentQueueSize() const
{
	return CurrentQueueSize;
}

uint32 ARTSStructure::GetMaxQueueSize() const
{
	return MaxQueueSize;
}

bool ARTSStructure::ScoreResource(TSubclassOf<AResource> ResourceType, int Amount, AActor* Donar)
{
	if (!IsDropPointFor(ResourceType)) return(false);

	/*TODO: Context for this function seems to be incorrect in Editor so we have to iterate through all World contexts till its valid*/
	/*it is valid in standalone though through normal GetWorld() call*/
	UWorld* World = GetWorld();
	ARTFPSGameState* GS = nullptr;

	if (World && World->WorldType != EWorldType::Game)
	{
		for (int i = 0; i < GEngine->GetWorldContexts().Num(); i++)
		{
			World = GEngine->GetWorldContexts()[i].World();
			if (World == nullptr) continue;

			GS = World->GetGameState<ARTFPSGameState>();
			if (GS != nullptr) break;
		}
	}
	else if(World)
	{
		GS = World->GetGameState<ARTFPSGameState>();
	}


	if (GS == nullptr) return false;

	return 	GS->AddTeamResource(GetTeam(), ResourceType, Amount);
}

bool ARTSStructure::PurchaseQueueItem(TSubclassOf<ARTSMinion> minionclass)
{
	UWorld * World = GetWorld();
	if (World == nullptr) return false;

	ARTFPSGameState* GS = World->GetGameState<ARTFPSGameState>();
	if (GS == nullptr) return false;

	int spawnindex = GetIndexByClass(minionclass);
	if (spawnindex >= 0)
	{
		return(GS->RemoveTeamResource(GetTeam(), SpawnableUnits[spawnindex].ResourceCost));
	}

	return false;
}

bool ARTSStructure::QueueMinion(TSubclassOf<ARTSMinion> minionclass, AController* InheritingController)
{
	int index = GetIndexByClass(minionclass);
	if (index < 0 || IsQueueFull()) return false;
	
	FStructureSpawnData Spawndata = SpawnableUnits[index];
		
	FStructureQueueData Queuedata;
	Queuedata.RecieveingController = InheritingController;
	Queuedata.SpawnClass = Spawndata.MinionClass;
	Queuedata.SpawnTime = SpawnableUnits[index].SpawnTime;
	
	if (StructureQueue.IsEmpty())
	{
		StructureQueue.Enqueue(Queuedata);
		CurrentQueueSize++;
		float spawntime = Queuedata.SpawnTime / 100.0f;
		GetWorldTimerManager().SetTimer(QueueHandler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
	}
	else
	{
		StructureQueue.Enqueue(Queuedata);
		CurrentQueueSize++;
	}


	return true;
}

void ARTSStructure::UpdateSpawnQueue()
{
	queuestatus++;
	FStructureQueueData queuedata = FStructureQueueData();
	if (queuestatus >= 100.0f)
	{ 
		if (StructureQueue.Dequeue(queuedata))
		{
			SpawnUnit(queuedata);
			CurrentQueueSize--;
		}
		
		queuestatus = 0.0f;
		
		if (StructureQueue.Peek(queuedata))
		{
			float spawntime = queuedata.SpawnTime / 100.0f;
			GetWorldTimerManager().SetTimer(QueueHandler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
		}
	}
	else if(StructureQueue.Peek(queuedata))
	{
		float spawntime = queuedata.SpawnTime / 100.0f;
		GetWorldTimerManager().SetTimer(QueueHandler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Updated Null Queue!")));
		queuestatus = 0.0f;
	}

}

void ARTSStructure::SpawnUnit(FStructureQueueData QueueData)
{
	UWorld * World = GetWorld();
	if (World == nullptr) return;

	ARTFPSGameState* GS = World->GetGameState<ARTFPSGameState>();
	if (GS == nullptr) return;

	GS->HandleStructureMinionSpawn(this, QueueData);
}

void ARTSStructure::CancelSpawn()
{

}
