// Fill out your copyright notice in the Description page of Project Settings.


#include "MineResourceAbilityUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/FPS/Abilities/MineResourceAbility.h"

void UMineResourceAbilityUpgrade::ApplyUpgrade(UObject * ToUpgrade) const
{
	IAbilityUserInterface * abilityuser = Cast<IAbilityUserInterface>(ToUpgrade);
	TArray<TWeakObjectPtr<UAbility>> abilities = GetTargetAbilitesFromUser(abilityuser);

	for (int i = 0; i < abilities.Num(); i++)
	{
		if (abilities[i].IsValid() == true)
		{
			UMineResourceAbility * ability = Cast<UMineResourceAbility>(abilities[i].Get());
			const float currentminerate = ability->GetMiningRate();
			const float newminerate = currentminerate - MineSpeedIncrement;
			ability->SetMiningRate(newminerate);


			const uint32 currentmineamount = ability->GetMineAmount();
			const uint32 newmineamount = currentmineamount + MineAmountIncrement;
			ability->SetMineAmount(newmineamount);
		}
	}
}