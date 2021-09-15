// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCommander.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "AbilityCombatCommander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AAbilityCombatCommander : public ACombatCommander, public IAbilityUserInterface
{
	GENERATED_BODY()
	
	virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;

};
