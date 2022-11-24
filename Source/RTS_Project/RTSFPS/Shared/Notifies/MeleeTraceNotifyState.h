// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "MeleeTraceNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMeleeTraceNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
		virtual void NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;



};
