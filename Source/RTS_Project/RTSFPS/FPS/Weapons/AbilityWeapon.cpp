// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWeapon.h"

AAbilityWeapon::AAbilityWeapon() : Super()
{
	AbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComp"));

}

void AAbilityWeapon::StartFire()
{
}

void AAbilityWeapon::StopFire()
{
}

void AAbilityWeapon::StartReload(bool bFromReplication)
{
}

void AAbilityWeapon::StopReload()
{
}


bool AAbilityWeapon::ServerStartUseAbility_Validate()
{
	return true;
}

void AAbilityWeapon::ServerStartUseAbility_Implementation()
{

}

bool AAbilityWeapon::ServerStopUseAbility_Validate()
{
	return true;
}

void AAbilityWeapon::ServerStopUseAbility_Implementation()
{

}