// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../AbilityComponent.h"
#include "AbilityAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UAbilityAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void InitializeAbilities(UAbilityComponent * InAbilComp);

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bLoopNotify = false;

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bReadyNotify = false;

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bEffectNotify = false;

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bEndNotify = false;

protected:
	UAbilityComponent * AbilityComp = nullptr;
};
