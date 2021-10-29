// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "RTSUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Ability.h"
#include "RTSAbilityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSAbilityUpgrade : public URTSUpgrade
{
	GENERATED_BODY()
	TArray<TSubclassOf<UAbility>> TargetAbilities;

};
