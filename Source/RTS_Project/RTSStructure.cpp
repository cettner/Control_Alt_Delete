// Fill out your copyright notice in the Description page of Project Settings.
#include "RTSStructure.h"
#include "GameFramework/PlayerController.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"
#include "RTSCatapult.h"
#include "RTSBUILDER.h"


// Sets default values
ARTSStructure::ARTSStructure(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (Can_Spawn_Builder)
	{
		FString path = "Blueprint'/Game/RTSMinion/RTSBUILDER.RTSBUILDER'";

		static ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(*path);

		if (TargetBlueprint.Object)
		{
			Builder = (UClass*)TargetBlueprint.Object->GeneratedClass;
		}
		else
		{

		}

	}
	if (Can_Spawn_Catapult)
	{
		FString path = "Blueprint'/Game/RTSMinion/RTSCatapult.RTSCatapult'";
		
		static ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(*path);

		if (TargetBlueprint.Object)
		{
			Catapult = (UClass*)TargetBlueprint.Object->GeneratedClass;
		}
		else
		{

		}
	}
}

// Called when the game starts or when spawned
void ARTSStructure::BeginPlay()
{
	PC = (ARTSPlayerController*)GetWorld()->GetFirstPlayerController();
	HudPtr = Cast<ARTSHUD>(PC->GetHUD());
	bIsConstructed = false;
	CurrentIntegrity = 1.0;

	BannerLocation = GetActorLocation();
	BannerLocation.X += spawndistance;
	SpawnLocation = BannerLocation;
}

// Called every frame
void ARTSStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ARTSStructure::IsDropPoint()
{
	return (isdroppoint);
}

void ARTSStructure::LoadSpawnableAsset(TSubclassOf<class ARTSMinion> &BP, FString path)
{

}

void ARTSStructure::Queue_Minion(int minion_index)
{
	if (minion_index > (int)UNITLBOUND && minion_index < (int)UNITUBOUND)
	{
		if (SpawnQueue.IsEmpty())
		{
			SpawnQueue.Enqueue(minion_index);
			float spawntime = GetSpawnTimeByIndex((Unit_Types)minion_index) / 100.0f;
			GetWorldTimerManager().SetTimer(Queue_Handler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
		}
		else
		{
			SpawnQueue.Enqueue(minion_index);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Minion Spawn Requested")));
	}
}

void ARTSStructure::UpdateSpawnQueue()
{
	Update_Queue_UI_Status(queuestatus);
	queuestatus++;
	int spawnindex = -1;
	if (queuestatus >= 100.0)
	{ 
		if (SpawnQueue.Dequeue(spawnindex))
		{
			SpawnUnit(spawnindex);
		}
		
		queuestatus = 0.0;
		
		if (SpawnQueue.Peek(spawnindex))
		{
			float spawntime = GetSpawnTimeByIndex((Unit_Types)spawnindex) / 100.0f;
			GetWorldTimerManager().SetTimer(Queue_Handler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
		}
	}
	else if(SpawnQueue.Peek(spawnindex))
	{
		float spawntime = GetSpawnTimeByIndex((Unit_Types)spawnindex) / 100.0f;
		GetWorldTimerManager().SetTimer(Queue_Handler, this, &ARTSStructure::UpdateSpawnQueue, 1.0, false, spawntime);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Updated Null Queue!")));
		queuestatus = 0.0;
	}

}

void ARTSStructure::SpawnUnit(int unit_index)
{
	UWorld* const World = GetWorld();
	Unit_Types type = (Unit_Types)unit_index;
	FRotator SpawnRotation(0, 0, 0);
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnLocation = GetActorLocation();
	SpawnLocation.X -= 500;
	SpawnLocation.Z = 100;

	if (type == CATAPULT && World  && Can_Spawn_Catapult)
	{
		ARTSCatapult * SpawnedCatapult = World->SpawnActor<ARTSCatapult>(Catapult, SpawnLocation, SpawnRotation, SpawnParams); 
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Catapult Spawned!")));
		PC->Update_UI_Spawn(SpawnedCatapult);
	}
	else if (type == BUILDER && World && Can_Spawn_Builder)
	{
		ARTSBUILDER * SpawnedBuilder = World->SpawnActor<ARTSBUILDER>(Builder, SpawnLocation, SpawnRotation, SpawnParams);
		PC->Update_UI_Spawn(SpawnedBuilder);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Builder Spawned!")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Minion Spawn Initialized")));
	}
	

	
}

void ARTSStructure::CancelSpawn()
{

}

float ARTSStructure::GetSpawnTimeByIndex(Unit_Types type)
{
	switch (type)
	{
	case BUILDER:
		return(Builder_Spawn_Time);
	case CATAPULT:
		return(Catapult_Spawn_Time);
	default:
		return - 1.0;
	}
}
