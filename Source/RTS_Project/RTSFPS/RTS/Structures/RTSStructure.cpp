// Fill out your copyright notice in the Description page of Project Settings.=
#include "RTSStructure.h"
#include "GameFramework/PlayerController.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"

// Sets default values
ARTSStructure::ARTSStructure(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARTSStructure::BeginPlay()
{
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
	Unit_Types type = (Unit_Types)unit_index;
	FRotator SpawnRotation(0, 0, 0);

	SpawnLocation = GetActorLocation();
	SpawnLocation.X -= 500;
	SpawnLocation.Z = 100;

//	ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetWorld()->GetFirstPlayerController());
//	PC->Spawn_RTS_Minion(SpawnLocation,SpawnRotation,type);

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
