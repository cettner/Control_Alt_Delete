// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSAbilityEnableUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"

void URTSAbilityEnableUpgrade::ApplyUpgrade(UObject * ToUpgrade) const
{
	IAbilityUserInterface * abilityuser = Cast<IAbilityUserInterface>(ToUpgrade);
	for (int i = 0; i < TargetAbilities.Num(); i++)
	{
		abilityuser->AddAbility(TargetAbilities[i]);
	}
}
