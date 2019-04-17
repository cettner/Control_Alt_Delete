// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/*Forward Declarations*/
class ARTSStructure;
class ARTSMinion;
class AWeapon;

UENUM(BlueprintType)
enum Structure_Types
{
	STRUCTURELBOUND,
	MINE,
	SPAWNER,
	STRUCTUREUBOUND
};

UENUM(BlueprintType)
enum Unit_Types
{
	UNITLBOUND = STRUCTUREUBOUND,
	BUILDER,
	CATAPULT,
	UNITUBOUND
};

UENUM(BlueprintType)
enum Weapon_Types
{
	WEAPONLBOUND = UNITUBOUND,
	EMPTY,
	SHIELD,
	WEAPONUBOUND
};

class RTS_PROJECT_API RTSSpawnHelper
{
public:
	RTSSpawnHelper();
	~RTSSpawnHelper();
	ARTSMinion * SpawnMinion(Unit_Types type, FVector SpawnLocation, FRotator SpawnRotation, UWorld *const World);
	ARTSStructure * SpawnStructure(Structure_Types type, FVector SpawnLocation, FRotator SpawnRotation, UWorld *const World);

private:
	bool InitializeSpawnableAsset(FString path, TSubclassOf<ARTSMinion> &asset);
	bool InitializeSpawnableAsset(FString path, TSubclassOf<ARTSStructure> &asset);
	
	static const FString BuilderPath;
	TSubclassOf<ARTSMinion> Builder;

	static const FString CatapultPath;
	TSubclassOf<ARTSMinion> Catapult;

	static const FString MinePath;
	TSubclassOf<ARTSStructure> Mine;

	static const FString SpawnerPath;
	TSubclassOf<ARTSStructure> Spawner;
};
