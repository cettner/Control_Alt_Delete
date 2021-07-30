// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"


/*Forward Declaration*/
class UAbilityComponent;

UCLASS()
class RTS_PROJECT_API UAbility : public UObject
{
	GENERATED_BODY()

	public:
		virtual void Init(UAbilityComponent * InComp);

		virtual void OnAbilityStart();
		virtual void OnEffect();
		virtual void OnAbilityReleased();
		virtual void OnAbilityEnd();


	protected:
		UAbilityComponent * AbilityComp;


};
