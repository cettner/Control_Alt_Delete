// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "..\..\Animation\MinionAnimInstance.h"
#include "BuilderAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBuilderAnimInstance : public UMinionAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsMining = false;

};

