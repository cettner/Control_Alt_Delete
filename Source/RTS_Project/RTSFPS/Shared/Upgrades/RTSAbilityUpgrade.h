// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "RTSUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Ability.h"
#include "RTSAbilityUpgrade.generated.h"



class IAbilityUserInterface;

UCLASS()
class RTS_PROJECT_API URTSAbilityUpgrade : public URTSUpgrade
{
	GENERATED_BODY()

public:
	virtual bool CanUpgrade(IUpgradableInterface * TestUpgrade) const override;
	TArray<TWeakObjectPtr<UAbility>> GetTargetAbilitesFromUser(IAbilityUserInterface * AbilityUser) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAbility>> TargetAbilities;

};
