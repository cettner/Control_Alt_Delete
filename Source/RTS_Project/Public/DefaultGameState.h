// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DefaultPlayerState.h"
#include "DefaultMode.h"
#include "DefaultGameState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultGameState : public AGameState
{
	GENERATED_BODY()
	
private:
	bool initialized = false;
	ADefaultMode * GM = nullptr;
	TArray<TArray<APlayerState *>> Teams;
	TArray<APlayerState *> Players;
	
private:
	bool TeamInitialize();
	int HasTeam(APlayerState * Player);

public:
	ADefaultGameState(const FObjectInitializer& ObjectInitializer);
	int AssignAvailableTeam(APlayerState * New_Player);
	virtual void OnRep_MatchState() override;
	
};
