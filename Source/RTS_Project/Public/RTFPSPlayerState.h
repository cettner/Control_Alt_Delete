// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultPlayerState.h"
#include "RTFPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTFPSPlayerState : public ADefaultPlayerState
{
	GENERATED_BODY()
	
public:
	bool isRtsPlayer = false;

protected:
	virtual void BeginPlay() override;
	
};
