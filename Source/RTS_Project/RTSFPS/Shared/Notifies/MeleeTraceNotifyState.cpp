// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeTraceNotifyState.h"
#include "..\Interfaces\CombatInterface.h"


void UMeleeTraceNotifyState::NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	Super::NotifyTick(InMeshComp, Animation, FrameDeltaTime);

	ICombatInterface * combatent = Cast<ICombatInterface>(InMeshComp->GetOwner());
	if (combatent != nullptr)
	{
		combatent->PerformTrace();
	}

}