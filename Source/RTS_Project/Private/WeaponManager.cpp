// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManager.h"
#include "Engine.h"

WeaponManager::WeaponManager()
{
}

WeaponManager::~WeaponManager()
{
}


AWeapon * WeaponManager::SpawnWeapon(Weapon_Types wep_type)
{
    FName socketname = GetSocketName(wep_type);
    const USkeletalMeshSocket * Weapon_Socket = Character->GetSocketByName(socketname);

    if(Weapon_Socket)
    {
       AWeapon * spawned_wep = PC->Spawn_Weapon(Weapon_Socket->RelativeLocation, Weapon_Socket->RelativeRotation,(int)Arsenal[loadout_index].PrimaryHand);
       //Weapon_Socket->AttachActor(spawned_wep,Character);
       return(spawned_wep);
    }
    
    return(nullptr);
}

bool WeaponManager::SpawnCurrentLoadOut()
{
    bool spawncomplete = false;

    if(initialized && loadout_index > -1)
    {
        if(Arsenal[loadout_index].PrimaryHand > NO_WEAPON)
        {
            PrimaryHand = SpawnWeapon(Arsenal[loadout_index].PrimaryHand);
            if(PrimaryHand)
            {
                spawncomplete = true;
            }
        }
        else
        {
            PrimaryHand = nullptr;
        }

        if(Arsenal[loadout_index].SecondaryHand > NO_WEAPON)
        {
            SecondaryHand = SpawnWeapon(Arsenal[loadout_index].SecondaryHand);
            if(SecondaryHand)
            {
                spawncomplete = true;
            }
        }
        else
        {
            SecondaryHand = nullptr;
        }
    }

    return(spawncomplete);
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
    if(initialized && loadout_index > -1)
    {
        return(Arsenal[loadout_index]);
    }
    else
    {
        WeaponLoadOut emptyloadout;
        return(emptyloadout);
    }
}

WeaponLoadOut WeaponManager::GetNextLoadOut()
{
	if (loadout_index > -1 && Arsenal.Num())
	{
		if (loadout_index == Arsenal.Num() -1)
		{
			loadout_index = 0;
			return(Arsenal[loadout_index]);
		}
		else
		{
			loadout_index++;
			return(Arsenal[loadout_index]);
		}
	}
	return WeaponLoadOut();
}

WeaponLoadOut WeaponManager::GetPreviousLoadOut()
{
	if (loadout_index > -1 && Arsenal.Num())
	{
		if (loadout_index == 0)
		{
			loadout_index = Arsenal.Num() - 1;
			return(Arsenal[loadout_index]);
		}
		else
		{
			loadout_index--;
			return(Arsenal[loadout_index]);
		}
	}
	return WeaponLoadOut();
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

FName WeaponManager::GetSocketName(const Weapon_Types type)
{
    return (SHIELD_SOCKET);
}