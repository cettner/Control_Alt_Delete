// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAnimNotify.h"
#include "../AbilityComponent.h"
#include "../Interfaces/AbilityUserInterface.h"


void UAbilityAnimNotify::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::Notify(MeshComp, Animation);


	IAbilityUserInterface * AbilityUser = Cast<IAbilityUserInterface>(MeshComp->GetOwner());
	
	if (AbilityUser != nullptr)
	{
		if(bReadyNotify == true)
		{
			AbilityUser->OnReadyNotify();
		}
		if (bLoopNotify == true)
		{
			AbilityUser->OnLoopNotify();
		}
		if (bEffectNotify == true)
		{
			AbilityUser->OnEffectNotify();
		}
		if(bEndNotify == true)
		{
			AbilityUser->OnEndNotify();
		}
	}


}
