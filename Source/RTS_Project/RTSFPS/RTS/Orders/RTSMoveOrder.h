// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSTargetedOrder.h"

#include "RTSMoveOrder.generated.h"


UCLASS(Blueprintable)
class RTS_PROJECT_API URTSMoveOrder : public URTSTargetedOrder
{
	GENERATED_BODY()

public:
	virtual void LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const override;
};
