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
	Selection = CreateDefaultSubobject<UDecalSelectionComponent>(TEXT("SelectionComp"));
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	if (MeshComp)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComp->SetCanEverAffectNavigation(true);
		MeshComp->bFillCollisionUnderneathForNavmesh = true;
		MeshComp->bReceivesDecals = false;
		Selection->SetDetection(MeshComp);
		Selection->SetupAttachment(RootComponent);
		SetDeselected();
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
}

void ARTSStructure::OnDeath()
{
	if (HasAuthority())
	{
		/*Unregister Minion to be percieved from AI perception*/
		UWorld* World = GetWorld();
		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
		PerceptionSystem->UnregisterSource(*this);


		/*Stop Ticking Events*/
		GetWorldTimerManager().ClearTimer(BuildUpdateHandler);
		GetWorldTimerManager().ClearTimer(QueueHandler);

		ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();
		GS->OnUnitDeath(this);
	}

	AClaimableSquareGameGrid* pgrid = Cast<AClaimableSquareGameGrid>(GetParentGrid());
	if (pgrid != nullptr)
	{
		for (int i = 0; i < Modifiers.Num(); i++)
		{
			pgrid->RemoveModifier(Modifiers[i], GridClaimSpace, this);
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
	Selection->SetHiddenInGame(false);
}

void ARTSStructure::SetDeselected()
{
	Selection->SetHiddenInGame(true);
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

bool ARTSStructure::CanSpawn(TSubclassOf<UObject> minionclass) const
{
	return (GetIndexByClass(minionclass) > -1);
}

int ARTSStructure::GetIndexByClass(TSubclassOf<UObject> minionclass) const
{
	int index = -1;
	for (int i = 0; i < SpawnableUnits.Num(); i++)
	{
		TSubclassOf<UObject> availableclasses = SpawnableUnits[i].SpawnClass;

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
	#if WITH_EDITOR
	const UWorld * world = GetWorldPIE();
	#else
	const UWorld * world = GetWorld();
	#endif // WITH_EDITOR

	ARTFPSGameState* gs = nullptr;
	gs = world->GetGameState<ARTFPSGameState>();

	check(gs);

	return 	gs->ScoreResource(ResourceType, Amount,this);
}

bool ARTSStructure::QueueActor(TSubclassOf<UObject> ObjectClass, AController* InheritingController)
{
	if (!HasAuthority()) return false;

	int index = GetIndexByClass(ObjectClass);
	if (index < 0 || IsQueueFull()) return false;
	
	FStructureSpawnData Spawndata = SpawnableUnits[index];
		
	FStructureQueueData Queuedata;
	Queuedata.RecieveingController = InheritingController;
	Queuedata.SpawnClass = Spawndata.SpawnClass;
	Queuedata.SpawnTime = SpawnableUnits[index].SpawnTime;
	
	if (StructureQueue.IsEmpty())
	{
		StructureQueue.Enqueue(Queuedata);
		CurrentQueueSize++;
		float spawntime = Queuedata.SpawnTime / 100.0f;
		#if WITH_EDITOR
		const UWorld * world = GetWorldPIE();
		#else
		const UWorld * world = GetWorld();
		#endif // WITH_EDITOR



		world->GetTimerManager().SetTimer(QueueHandler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
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
			#if WITH_EDITOR
			const UWorld * world = GetWorldPIE();
			#else
			const UWorld * world = GetWorld();
			#endif // WITH_EDITOR
			float spawntime = queuedata.SpawnTime / 100.0f;
			world->GetTimerManager().SetTimer(QueueHandler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
		}
	}
	else if(StructureQueue.Peek(queuedata))
	{
		#if WITH_EDITOR
		const UWorld * world = GetWorldPIE();
		#else
		const UWorld * world = GetWorld();
		#endif // WITH_EDITOR
		float spawntime = queuedata.SpawnTime / 100.0f;
		world->GetTimerManager().SetTimer(QueueHandler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Updated Null Queue!")));
		queuestatus = 0.0f;
	}

}

void ARTSStructure::SpawnUnit(FStructureQueueData QueueData)
{
	#if WITH_EDITOR
	const UWorld * world = GetWorldPIE();
	#else
	const UWorld * world = GetWorld();
	#endif // WITH_EDITOR
	if (world == nullptr) return;

	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	if (gs == nullptr) return;

	gs->SpawnObjectFromStructure(this, QueueData);
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

#if WITH_EDITOR
UWorld * ARTSStructure::GetWorldPIE() const
{

	UWorld * world = GetWorld();
	if ( world && world->WorldType == EWorldType::PIE) return world;

	for (int i = 0; i < GEngine->GetWorldContexts().Num(); i++)
	{
		world = GEngine->GetWorldContexts()[i].World();
		if (world == nullptr) continue;
		else if (world->WorldType == EWorldType::PIE)
		{
			return world;
		}

	}
	return world;
}
#endif