// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAnimNotifyState.h"

void UAbilityAnimNotifyState::NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference)
{
	Super::NotifyTick(InMeshComp, InAnimation, InFrameDeltaTime, InEventReference);
}
