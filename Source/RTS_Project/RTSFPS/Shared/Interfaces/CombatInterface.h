// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Notifies/MeleeTraceNotifyState.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

constexpr int32 CANT_ATTACK_INDEX = -1;

class RTS_PROJECT_API ICombatInterface
{
	GENERATED_BODY()

	friend UMeleeTraceNotifyState;

public:
	virtual bool IsAttacking() const;
	virtual bool StartAttack(const int32 InAttackID = CANT_ATTACK_INDEX);

protected:
	virtual FHitResult PerformTrace();
	virtual int32 GetAttackIndexForTarget(const AActor* InToAttack) const;
};
