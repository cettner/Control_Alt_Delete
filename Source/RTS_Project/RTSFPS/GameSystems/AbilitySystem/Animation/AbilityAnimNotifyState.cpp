// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAnimNotifyState.h"
#include "../Interfaces/AbilityUserInterface.h"

void UAbilityAnimNotifyState::NotifyTick(USkeletalMeshComponent* InMeshComp, UAnimSequenceBase* InAnimation, float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference)
{
	Super::NotifyTick(InMeshComp, InAnimation, InFrameDeltaTime, InEventReference);

	IAbilityUserInterface* abilityuser = Cast<IAbilityUserInterface>(InMeshComp->GetOwner());

	if (abilityuser != nullptr)
	{
		abilityuser->OnTickNotify(InFrameDeltaTime, InEventReference);
	}
}
