// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManager.h"
#include "Engine.h"

WeaponManager::WeaponManager()
{
}

WeaponManager::~WeaponManager()
{
}

void WeaponManager::AddLoadout(WeaponLoadOut Added_Set)
{

}

bool WeaponManager::Initialize(ARTSPlayerController * controller, USkeletalMeshComponent * character_mesh)
{
    initialized = false;
    if(controller && character_mesh)
    {
        Character = character_mesh;
        PC = controller;
        Arsenal.Empty();
        for(int i = 0; i < inventory_size; i++)
        {
            WeaponLoadOut emptyloadout;
            Arsenal.Add(emptyloadout);
        }    
        initialized = true;
    }
    else
    {
	    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("WeaponManager Failed to Initialize!")));
    }
    return (initialized);
}