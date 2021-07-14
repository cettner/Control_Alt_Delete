// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "StructureAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UStructureAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	


protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsBuilt = false;

};
