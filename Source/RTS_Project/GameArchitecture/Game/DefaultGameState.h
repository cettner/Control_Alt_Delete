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
	
protected:
	bool initialized = false;
	ADefaultMode * GM = nullptr;
	TArray<TArray<APlayerState *>> Teams;
	

public:
	virtual bool TeamInitialize(ADefaultMode * GameMode);
	int AssignAvailableTeam(APlayerState * New_Player);
	int AssignBalancedTeam(APlayerState * New_Player);
	bool SwapTeam(APlayerState * Player, int New_Team);
	int HasTeam(APlayerState * Player) const;
	bool IsTeamValid(int team_id) const;
	bool IsTeamFull(int Team_Index) const;
	bool LeaveTeam(APlayerState * Player);
	
};
