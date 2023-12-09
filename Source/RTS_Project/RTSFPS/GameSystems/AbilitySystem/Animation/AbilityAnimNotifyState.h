// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AbilityAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UAbilityAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference) override;
};
