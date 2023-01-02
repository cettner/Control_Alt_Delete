// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAnimNotify.h"
#include "../AbilityComponent.h"
#include "../Interfaces/AbilityUserInterface.h"


FName UAbilityAnimNotify::GetAssetSlotName() const
{
	return(AnimTrackSlotName);
}

void UAbilityAnimNotify::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AnimTrackSlotName == "")
	{
		UAnimMontage * Montage = Cast<UAnimMontage>(Animation);
		FSlotAnimationTrack track = Montage->SlotAnimTracks[0];
		AnimTrackSlotName = track.SlotName;
	}


	IAbilityUserInterface * AbilityUser = Cast<IAbilityUserInterface>(MeshComp->GetOwner());
	
	if (AbilityUser != nullptr)
	{
		if(bReadyNotify == true)
		{
			AbilityUser->OnReadyNotify(this);
		}
		if (bLoopNotify == true)
		{
			AbilityUser->OnLoopNotify(this);
		}
		if (bEffectNotify == true)
		{
			AbilityUser->OnEffectNotify(this);
		}
		if(bEndNotify == true)
		{
			AbilityUser->OnEndNotify(this);
		}
	}
}
