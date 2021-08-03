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

	}


}
