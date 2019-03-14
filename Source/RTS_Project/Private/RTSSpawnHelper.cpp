// Fill out your copyright notice in the Description page of Project Settings.
#include "RTSSpawnHelper.h"
#include "RTSCatapult.h"
#include "RTSBUILDER.h"
#include "RTSStructure.h"

RTSSpawnHelper::RTSSpawnHelper()
{
FString path = "Blueprint'/Game/RTSMinion/RTSBUILDER.RTSBUILDER'";
InitializeSpawnableAsset(path, Builder);

path = "Blueprint'/Game/RTSMinion/RTSCatapult.RTSCatapult'";
InitializeSpawnableAsset(path, Catapult);
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

bool InitializeSpawnableAsset(FString path, TSubclassOf<ARTSStructure> &asset)
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