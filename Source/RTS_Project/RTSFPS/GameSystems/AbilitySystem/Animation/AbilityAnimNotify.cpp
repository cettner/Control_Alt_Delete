// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAnimNotify.h"



void UAbilityAnimNotify::InitializeAbilities(UAbilityComponent * InAbilComp)
{
	AbilityComp = InAbilComp;
}


void UAbilityAnimNotify::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (IsValid(AbilityComp))
	{
		if(bReadyNotify == true)
		{
			AbilityComp->OnReadyNotify();
		}
		if (bLoopNotify == true)
		{
			AbilityComp->OnLoopNotify();
		}
		if (bEffectNotify == true)
		{
			AbilityComp->OnEffectNotify();
		}
		if(bEndNotify == true)
		{
			AbilityComp->OnEndNotify();
		}
	}


}
