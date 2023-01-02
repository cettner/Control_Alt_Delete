// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AbilityAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UAbilityAnimNotify : public UAnimNotify
{
	GENERATED_BODY()


public:
	virtual FName GetAssetSlotName() const;

protected:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	FName AnimTrackSlotName = "";

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bLoopNotify = false;

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bReadyNotify = false;

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bEffectNotify = false;

	UPROPERTY(EditAnyWhere, Category = Ability)
	bool bEndNotify = false;
};
