// Fill out your copyright notice in the Description page of Project Settings.=
#include "RTSStructure.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"
#include "UI/StructureSpawnQueueWidget.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/ClaimableSquareGameGrid.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ARTSStructure::ARTSStructure() : Super() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
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

	if (HasAuthority())
	{
		/*Set Up perception so that it can be sensed by AI Pawnsensing Agents*/
		UWorld* World = GetWorld();
		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
		if (PerceptionSystem)
		{
			PerceptionSystem->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
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


	/*Pick a Random Destruction Animation to Play on Death*/
	if (DestroyAnimations.Num())
	{
		UAnimMontage* DestroyAnimation;
		uint8_t destroyindex = abs(rand() % DestroyAnimations.Num());
		DestroyAnimation = DestroyAnimations[destroyindex];
		Health->SetDeathanimMontage(DestroyAnimation);
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

FTransform ARTSStructure::FindActorSpawnLocation(FVector InBoxExtent) const
{
	FTransform retval = GetTransform();
	bool foundlocation = false;

	/*Unimportant stuff*/
	#if WITH_EDITOR
		 UWorld* world = GetWorldPIE();
	#else
		 UWorld* world = GetWorld();
	#endif // WITH_EDITOR

	/*Create a Box with same extents as the incoming unit*/
	const FCollisionShape traceshape = FCollisionShape::MakeBox(InBoxExtent);
	
	FCollisionQueryParams queryparams = FCollisionQueryParams::DefaultQueryParam;
	FName TraceTag("DebugShooterTag");
	world->DebugDrawTraceTag = TraceTag;
	queryparams.TraceTag = TraceTag;
	queryparams.bTraceComplex = false;
	queryparams.bReturnFaceIndex = true;
	queryparams.bReturnPhysicalMaterial = false;
	queryparams.bFindInitialOverlaps = false;

	/*Distance the box is "moved" per check, will probably make this configurable, but for now make the quarter-width of the incoming volume*/
	const float adjustmentdist = InBoxExtent.X;
	/*Small collision buffer added to Trace check positions so we dont collide with it directly, Don't want to add to ignored actors*/
	const float collisionbuffer = 10.0f;
	
	/*Ideally the center of the object, can also use AActor::GetComponentsBoundingBox.GetOrigin(), for true relative center*/
	const FVector actororigin = GetActorLocation();
	/*Half width for the structures Bounding Area in x,y,z*/
	const FVector actorbounds = GetComponentsBoundingBox(false, false).GetExtent();

	const float totalwidth = actorbounds.X * 2.0f;
	const float totallength = actorbounds.Y * 2.0f;
	/*Measure sides of bounding area, + 2 units on each end for corners then discern how many boxes can be drawn of that size along the side*/
	const int32 numboxesperwidth = FMath::FloorToInt((totalwidth + (2 * InBoxExtent.X)) / adjustmentdist);
	const int32 numboxesperheight = FMath::FloorToInt((totallength + (2 * InBoxExtent.Y)) / adjustmentdist);

	/*Four starting corners of the bounding area such that the corner of the spawning structure touches the corner of the spawned units bouning area*/
	const FVector topLeftcornerstart = FVector(actororigin.X + actorbounds.X + InBoxExtent.X + collisionbuffer, actororigin.Y + actorbounds.Y + InBoxExtent.Y, InBoxExtent.Z);
	const FVector toprightcornerstart = FVector(actororigin.X + actorbounds.X + InBoxExtent.X, actororigin.Y - actorbounds.Y - InBoxExtent.Y - collisionbuffer, InBoxExtent.Z);
	const FVector bottomrightcornerstart = FVector(actororigin.X - actorbounds.X - InBoxExtent.X - collisionbuffer, actororigin.Y - actorbounds.Y - InBoxExtent.Y, InBoxExtent.Z);
	const FVector bottomleftcornerstart = FVector(actororigin.X - actorbounds.X - InBoxExtent.X, actororigin.Y + actorbounds.Y + InBoxExtent.Y + collisionbuffer, InBoxExtent.Z);

	const FQuat tracerotation = FRotator(0.0f, 0.0f, 0.0f).Quaternion();

	FVector traceboxorigin;
	FHitResult outhit;

	for (int i = 0; i < numboxesperheight; i++)
	{
		traceboxorigin = topLeftcornerstart - FVector(0.0f, adjustmentdist * i, 0.0f);
		
		world->SweepSingleByChannel(outhit, traceboxorigin, traceboxorigin + FVector(0, 0, 1), tracerotation, SpawnTraceChannel, traceshape, queryparams);
		if (outhit.bBlockingHit == false)
		{
			/*Nothing detected in bounding area so good to break from the loop*/
			retval.SetLocation(traceboxorigin);
			foundlocation = true;
			break;
		}
	}

	if (!foundlocation)
	{
		for (int i = 0; i < numboxesperwidth; i++)
		{
			traceboxorigin = toprightcornerstart - FVector(adjustmentdist * i, 0.0f, 0.0f);

			world->SweepSingleByChannel(outhit, traceboxorigin, traceboxorigin + FVector(0, 0, 1), tracerotation, SpawnTraceChannel, traceshape, queryparams);

			if (outhit.bBlockingHit == false)
			{
				/*Nothing detected in bounding area so good to break from the loop*/
				retval.SetLocation(traceboxorigin);
				foundlocation = true;
				break;
			}
		}
	}

	if (!foundlocation)
	{
		for (int i = 0; i < numboxesperheight; i++)
		{
			traceboxorigin = bottomrightcornerstart + FVector(0.0f, adjustmentdist * i, 0.0f);

			world->SweepSingleByChannel(outhit, traceboxorigin, traceboxorigin + FVector(0, 0, 1), tracerotation, SpawnTraceChannel, traceshape, queryparams);

			if (outhit.bBlockingHit == false)
			{
				/*Nothing detected in bounding area so good to break from the loop*/
				retval.SetLocation(traceboxorigin);
				foundlocation = true;
				break;
			}
		}
	}

	if (!foundlocation)
	{
		for (int i = 0; i < numboxesperwidth; i++)
		{
			traceboxorigin = bottomleftcornerstart + FVector(adjustmentdist * i, 0.0f, 0.0f);

			world->SweepSingleByChannel(outhit, traceboxorigin, traceboxorigin + FVector(0, 0, 1), tracerotation, SpawnTraceChannel, traceshape, queryparams);
			if (outhit.bBlockingHit == false)
			{
				/*Nothing detected in bounding area so good to break from the loop*/
				retval.SetLocation(traceboxorigin);
				foundlocation = true;
				break;
			}
		}
	}
	
	return retval;
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
	const UWorld* world = GetWorld();

	const APlayerController* pc = world->GetFirstPlayerController<APlayerController>();

	ADefaultPlayerState* ps = pc->GetPlayerState<ADefaultPlayerState>();
	if (ps && ps->TeamID != TeamIndex)
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