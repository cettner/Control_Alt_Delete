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
	
	public:
		int32 GetMaxSegmentHitCount() const { return MaxSegmentHitCount; }

	protected:
		virtual void NotifyBegin(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, float InTotalDuration, const FAnimNotifyEventReference& InEventReference) override;
		virtual void NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference) override;
		virtual void NotifyEnd(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, const FAnimNotifyEventReference& InEventReference) override;

	protected:
		/*Configurable Max times the User Should trace for this segment*/
		UPROPERTY(EditAnyWhere)
		int32 MaxSegmentHitCount = 1;
};
