// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DefaultPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	bool isRtsPlayer = false;
	int Team_ID = -1;
	
};
