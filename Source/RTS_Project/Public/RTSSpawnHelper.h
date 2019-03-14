// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSStructure.h"
#include "RTSMinion.h"


class RTS_PROJECT_API RTSSpawnHelper
{
public:
	RTSSpawnHelper();
	~RTSSpawnHelper();
	bool InitializeSpawnableAsset(FString path, TSubclassOf<ARTSMinion> &asset);
	bool InitializeSpawnableAsset(FString path, TSubclassOf<ARTSStructure> &asset);
	
private:
	TSubclassOf<ARTSMinion> Builder;
	TSubclassOf<ARTSMinion> Catapult;
	TSubclassOf<ARTSStructure> Mine;
	TSubclassOf<ARTSStructure> Spawner;
};
