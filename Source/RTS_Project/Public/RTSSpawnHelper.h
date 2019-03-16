// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/*Forward Declarations*/
class ARTSStructure;
class ARTSMinion;

enum Structure_Types
{
	STRUCTURELBOUND,
	MINE,
	SPAWNER,
	STRUCTUREUBOUND
};

enum Unit_Types
{
	UNITLBOUND = STRUCTUREUBOUND,
	BUILDER,
	CATAPULT,
	UNITUBOUND
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

	const FString BuilderPath = "Blueprint'/Game/RTSMinion/RTSBUILDER.RTSBUILDER'";
	TSubclassOf<ARTSMinion> Builder;

	const FString CatapultPath = "Blueprint'/Game/RTSMinion/RTSCatapult.RTSCatapult'";
	TSubclassOf<ARTSMinion> Catapult;

	const FString MinePath = "Blueprint'/Game/TopDownBP/Actors/Structures/MINE_BP.MINE_BP'";
	TSubclassOf<ARTSStructure> Mine;

	const FString SpawnerPath = "Blueprint'/Game/TopDownBP/Actors/Structures/Spawner_BP.Spawner_BP'";
	TSubclassOf<ARTSStructure> Spawner;
};
