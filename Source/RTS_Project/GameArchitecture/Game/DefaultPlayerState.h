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
	ADefaultPlayerState(const FObjectInitializer& ObjectInitializer);

protected:
	/*Virtual Function For Override*/
	UFUNCTION()
	virtual void OnRep_TeamID();

protected:
	virtual void PostInitializeComponents() override;
	void ClientInitialize(AController* Controller) override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


public:

	UPROPERTY(ReplicatedUsing = OnRep_TeamID)
	int TeamID;
	
};
