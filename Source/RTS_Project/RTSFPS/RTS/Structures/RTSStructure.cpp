// Fill out your copyright notice in the Description page of Project Settings.=
#include "RTSStructure.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"
#include "UI/StructureSpawnQueueWidget.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/ClaimableSquareGameGrid.h"

#include "Components/DecalComponent.h"
#include "Materials/Material.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARTSStructure::ARTSStructure() : Super() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Selection = CreateDefaultSubobject<URTSSelectionComponent>(TEXT("SelectionComp"));
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	if (MeshComp)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCanEverAffectNavigation(true);
		MeshComp->bFillCollisionUnderneathForNavmesh = true;
		MeshComp->bReceivesDecals = false;

		Selection->SetDetection(MeshComp);
		Selection->SetRoot(MeshComp);
	}

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->OnDeathStart.BindUFunction(this, "OnDeath");
	Health->SetIsReplicated(true);

	MenuClass = nullptr;
	Menu = nullptr;
}

void ARTSStructure::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*Set Up perception so that it can be sensed by AI Pawnsensing Agents*/
	UWorld* World = GetWorld();
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
	if (PerceptionSystem)
	{
		PerceptionSystem->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
	}

	/*Pick a Random Destruction Animation to Play on Death*/
	if (DestroyAnimations.Num())
	{
		UAnimMontage* DestroyAnimation;
		uint8_t destroyindex = abs(rand() % DestroyAnimations.Num());
		DestroyAnimation = DestroyAnimations[destroyindex];
		Health->SetDeathanimMontage(DestroyAnimation);
	}

	if (!bSkipsConstruction)
	{
		Health->SetCurrentHealth(UnConstructedHealth);
		bISConstructed = false;
		BeginConstruction();
	}
	else
	{
		OnConstructionComplete();
	}

}

UUserWidget* ARTSStructure::GetMenu()
{
	UWorld* World = GetWorld();

	if (World == nullptr || !IsConstructed()) return nullptr;

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
	if (minion && IsConstructed())
	{
		return(GetTeam() == minion->GetTeam());
	}
	return false;
}

float ARTSStructure::GetPercentConstructed() const
{
	return PercentConstructed;
}

bool ARTSStructure::IsConstructed() const
{
	return bISConstructed;
}

void ARTSStructure::BeginConstruction()
{
	if (bAutoBuilds)
	{
		BuildUpdateDelegate = FTimerDelegate::CreateUObject(this, &ARTSStructure::IncrementConstruction, (float)AutoBuildPerTick, (AActor*)this);
		GetWorldTimerManager().SetTimer(BuildUpdateHandler, BuildUpdateDelegate, AutoBuildRate, true);
	}
}

void ARTSStructure::IncrementConstruction(float DeltaConstruction, AActor * Contributor)
{
	PercentConstructed += DeltaConstruction;

	if (PercentConstructed >= CONSTRUCTION_COMPLETE_THRESHOLD)
	{
		GetWorldTimerManager().ClearTimer(BuildUpdateHandler);
		OnConstructionComplete();
	}
}

void ARTSStructure::OnConstructionComplete()
{
	bISConstructed = true;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("Structure Complete!!")));
}

void ARTSStructure::OnDeath()
{
	/*Unregister Minion to be percieved from AI perception*/
	UWorld* World = GetWorld();
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
	PerceptionSystem->UnregisterSource(*this);

	ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();
	GS->OnUnitDeath(this);

	AClaimableSquareGameGrid* pgrid = Cast<AClaimableSquareGameGrid>(GetParentGrid());
	if (pgrid != nullptr)
	{
		for (int i = 0; i < Modifiers.Num(); i++)
		{
			pgrid->RemoveModifier(Modifiers[i],GridClaimSpace,this);
		}
	}
}

float ARTSStructure::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ProcessedDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	ProcessedDamage = Health->HandleDamageEvent(ProcessedDamage, DamageEvent, EventInstigator, DamageCauser);

	return (ProcessedDamage);
}

void ARTSStructure::SetSelected()
{
	Selection->SetSelected();
}

void ARTSStructure::SetDeselected()
{
	Selection->SetDeselected();
}

int ARTSStructure::GetTeam() const
{
	return TeamIndex;
}

void ARTSStructure::SetTeam(int newteamindex)
{
	TeamIndex = newteamindex;
}

void ARTSStructure::SetTeamColors(FLinearColor TeamColor)
{
	if (Selection)
	{
		Selection->SetSelectionColor(TeamColor);
	}
}

bool ARTSStructure::IsDropPointFor(TSubclassOf<AResource> ResourceType) const
{
	return (Health->IsAlive());
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
		return(GS->RemoveTeamResource(GetTeam(), SpawnableUnits[spawnindex].ResourceCost.GetMap()));
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

void ARTSStructure::OnRep_TeamIndex()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ADefaultPlayerController* PC = World->GetFirstPlayerController<ADefaultPlayerController>();
	if (PC == nullptr) return;

	ADefaultPlayerState* PS = PC->GetPlayerState<ADefaultPlayerState>();
	if (PS && PS->TeamID != TeamIndex)
	{
		SetTeamColors(FLinearColor::Red);
		SetSelected();
	}
	else
	{
		SetDeselected();
	}

}

void  ARTSStructure::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSStructure, queuestatus);
	DOREPLIFETIME(ARTSStructure, TeamIndex);
}
