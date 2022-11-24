// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSTargetedOrder.h"

#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTSAttackOrder.generated.h"


UCLASS(Blueprintable)
class RTS_PROJECT_API URTSAttackOrder : public URTSTargetedOrder
{
	GENERATED_BODY()

public:
	FORCEINLINE TScriptInterface<IRTSObjectInterface> GetTarget() const { return Target; }
	virtual void SetTargetContext(AController* Issuer, const FHitResult& InContext) override;
	virtual void LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const override;

	protected:
	TScriptInterface<IRTSObjectInterface> Target = nullptr;
};
