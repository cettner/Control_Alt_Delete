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

protected:
	virtual void PostInitializeComponents() override;
protected:
	virtual void BeginPlay() override;
	virtual void ClientInitialize(AController * C) override;

public:
	ADefaultPlayerState(const FObjectInitializer& ObjectInitializer);

public:


	int Team_ID = -1;
	
};
