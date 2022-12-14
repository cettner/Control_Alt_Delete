// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MinionAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMinionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

/**************************Third Person Mesh***************************/
protected:
	UPROPERTY(BlueprintReadOnly)
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float Direction = 0.0f;
/***********************************************************************/

};

