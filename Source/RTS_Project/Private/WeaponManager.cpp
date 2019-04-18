// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManager.h"
#include "Engine.h"

WeaponManager::WeaponManager()
{
}

WeaponManager::~WeaponManager()
{
}

bool WeaponManager::AddLoadout(WeaponLoadOut Added_Set)
{
    if(Arsenal.Num() < max_inventory_size)
    {
        Arsenal.Add(Added_Set);
        if(loadout_index == -1)
        {
            loadout_index = 0;
        }

        return(true);
    }
    return(false);
}

WeaponLoadOut WeaponManager::GetCurrentLoadOut()
{
    if(initialized && loadout_index != -1)
    {
        return(Arsenal[loadout_index]);
    }
    else
    {
        WeaponLoadOut emptyloadout;
        return(emptyloadout);
    }
}

bool WeaponManager::Initialize(ARTSPlayerController * controller, USkeletalMeshComponent * character_mesh)
{
    initialized = false;
    if(controller && character_mesh)
    {
        Character = character_mesh;
        PC = controller;
        Arsenal.Empty();
        loadout_index = -1;
        initialized = true;
    }
    else
    {
	    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("WeaponManager Failed to Initialize!")));
    }
    return (initialized);
}