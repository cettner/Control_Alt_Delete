// Fill out your copyright notice in the Description page of Project Settings.
#include "RTSSpawnHelper.h"
#include "RTSStructure.h"
#include "RTSMinion.h"
#include "RTSCatapult.h"
#include "RTSBUILDER.h"
#include "Engine.h"

RTSSpawnHelper::RTSSpawnHelper()
{
bool assetsloaded = true;
assetsloaded &= InitializeSpawnableAsset(BuilderPath, Builder);

assetsloaded &= InitializeSpawnableAsset(CatapultPath, Catapult);

assetsloaded &= InitializeSpawnableAsset(MinePath, Mine);

assetsloaded &= InitializeSpawnableAsset(SpawnerPath, Spawner);

	if (!assetsloaded)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Assets Failed To Load!")));
	}

}

RTSSpawnHelper::~RTSSpawnHelper()
{
}

bool RTSSpawnHelper::InitializeSpawnableAsset(FString path, TSubclassOf<ARTSMinion> &asset)
{
    static ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(*path);
    if (TargetBlueprint.Object)
	{
	    asset = (UClass*)TargetBlueprint.Object->GeneratedClass;
        return true;
	}
    else
    {
        return false;
    }
   
}

bool RTSSpawnHelper::InitializeSpawnableAsset(FString path, TSubclassOf<ARTSStructure> &asset)
{
    static ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(*path);
    if (TargetBlueprint.Object)
	{
		asset = (UClass*)TargetBlueprint.Object->GeneratedClass;
        return true;
	}
    else
    {
        return false;
    }
   
}

ARTSMinion * RTSSpawnHelper::SpawnMinion(Unit_Types type, FVector SpawnLocation, FRotator SpawnRotation, UWorld *const World)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (type == CATAPULT && World)
	{
		ARTSCatapult * SpawnedCatapult = World->SpawnActor<ARTSCatapult>(Catapult, SpawnLocation, SpawnRotation, SpawnParams);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Catapult Spawned!")));
		return(SpawnedCatapult);
	}
	else if (type == BUILDER && World)
	{
		ARTSBUILDER * SpawnedBuilder = World->SpawnActor<ARTSBUILDER>(Builder, SpawnLocation, SpawnRotation, SpawnParams);
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Builder Spawned!")));
		return(SpawnedBuilder);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Minion Spawn Requested")));
	}

	return nullptr;
}
