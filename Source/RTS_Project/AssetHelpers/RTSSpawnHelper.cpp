// Fill out your copyright notice in the Description page of Project Settings.
#include "RTSSpawnHelper.h"
#include "../RTSFPS/RTS/Minions/Catapult/RTSCatapult.h"
#include "../RTSFPS/RTS/Minions/Catapult/RTSCatapult.h"
#include "../RTSFPS/RTS/Minions/Builder/RTSBUILDER.h"
#include "../RTSFPS/FPS/Weapons/Weapon.h"
#include "GameAssets.h"
#include "Engine.h"

const FString RTSSpawnHelper::BuilderPath = TEXT(BUILDER_BP_PATH);
const FString RTSSpawnHelper::CatapultPath = TEXT(CATAPULT_BP_PATH);
const FString RTSSpawnHelper::MinePath = TEXT(MINE_BP_PATH);
const FString RTSSpawnHelper::SpawnerPath = TEXT(SPAWNER_BP_PATH);
const FString RTSSpawnHelper::ShieldPath = TEXT(SHIELD_BP_PATH);

RTSSpawnHelper::RTSSpawnHelper()
{
bool assetsloaded = true;
assetsloaded &= InitializeSpawnableAsset(BuilderPath, Builder);

assetsloaded &= InitializeSpawnableAsset(CatapultPath, Catapult);

assetsloaded &= InitializeSpawnableAsset(MinePath, Mine);

assetsloaded &= InitializeSpawnableAsset(SpawnerPath, Spawner);

assetsloaded &= InitializeSpawnableAsset(ShieldPath, Shield);

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
    ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(*path);
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
    ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(*path);
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

bool RTSSpawnHelper::InitializeSpawnableAsset(FString path, TSubclassOf<AWeapon> &asset)
{
    ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(*path);
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

ARTSStructure * RTSSpawnHelper::SpawnStructure(Structure_Types type, FVector SpawnLocation, FRotator SpawnRotation, UWorld *const World)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (type == MINE  && World)
	{
			ARTSStructure * SpawnedMine = World->SpawnActor<ARTSStructure>(Mine, SpawnLocation, SpawnRotation, SpawnParams);
			return(SpawnedMine);
	}
	else if(type == SPAWNER && World)
	{
		ARTSStructure * SpawnedSpawner = World->SpawnActor<ARTSStructure>(Spawner, SpawnLocation, SpawnRotation, SpawnParams);
		return(SpawnedSpawner);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Structure Spawn Requested")));
	}

	return nullptr;
}

AWeapon * RTSSpawnHelper::SpawnWeapon(Weapon_Types type,  FVector SpawnLocation, FRotator SpawnRotation, UWorld *const World)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(type == SHIELD && World)
	{
		AWeapon * SpawnedWeapon = World->SpawnActor<AWeapon>(Shield, SpawnLocation, SpawnRotation, SpawnParams);
		return(SpawnedWeapon);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Weapon Spawn Requested")));
	}
	return nullptr;
}