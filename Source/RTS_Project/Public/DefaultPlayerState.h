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
	/** Used to tell UE which variables to replicate */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

public:
	ADefaultPlayerState(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(Replicated)
	bool isRtsPlayer = false;
	
	UPROPERTY(Replicated)
	int Team_ID = -1;
	
};
