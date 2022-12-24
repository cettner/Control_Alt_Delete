// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeTraceNotifyState.h"
#include "..\Interfaces\CombatInterface.h"


void UMeleeTraceNotifyState::NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(InMeshComp, Animation, FrameDeltaTime);

	ICombatInterface * combatent = Cast<ICombatInterface>(InMeshComp->GetOwner());
	if (combatent != nullptr  && combatent->ShouldPerformTrace())
	{
		combatent->PerformTrace();
	}
}

void UMeleeTraceNotifyState::NotifyEnd(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(InMeshComp, Animation);
	ICombatInterface* combatent = Cast<ICombatInterface>(InMeshComp->GetOwner());
	if (combatent != nullptr)
	{
		combatent->OnAttackSegmentEnd(this);
	}
}

void UMeleeTraceNotifyState::NotifyBegin(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(InMeshComp, Animation, TotalDuration);
	ICombatInterface* combatent = Cast<ICombatInterface>(InMeshComp->GetOwner());
	if (combatent != nullptr)
	{
		combatent->OnAttackSegmentStart(this);
	}
}
