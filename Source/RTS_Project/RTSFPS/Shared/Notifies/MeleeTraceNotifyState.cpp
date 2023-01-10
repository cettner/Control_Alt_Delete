// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeTraceNotifyState.h"
#include "..\Interfaces\CombatInterface.h"


void UMeleeTraceNotifyState::NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference)
{
	Super::NotifyTick(InMeshComp, InAnimation, InFrameDeltaTime, InEventReference);

	ICombatInterface * combatent = Cast<ICombatInterface>(InMeshComp->GetOwner());
	if (combatent != nullptr  && combatent->ShouldPerformTrace())
	{
		combatent->PerformTrace();
	}
}

void UMeleeTraceNotifyState::NotifyEnd(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, const FAnimNotifyEventReference& InEventReference)
{
	Super::NotifyEnd(InMeshComp, InAnimation, InEventReference);
	ICombatInterface* combatent = Cast<ICombatInterface>(InMeshComp->GetOwner());
	if (combatent != nullptr)
	{
		combatent->OnAttackSegmentEnd(this);
	}
}

void UMeleeTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	ICombatInterface* combatent = Cast<ICombatInterface>(MeshComp->GetOwner());
	if (combatent != nullptr)
	{
		combatent->OnAttackSegmentStart(this);
	}
}
