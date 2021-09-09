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
	


public:
	virtual bool TeamInitialize(ADefaultMode * GameMode);
	int AssignAvailableTeam(APlayerState * New_Player);
	int AssignBalancedTeam(APlayerState * New_Player);
	bool SwapTeam(APlayerState * Player, int New_Team);
	int HasTeam(APlayerState * Player) const;
	bool IsTeamValid(int team_id) const;
	bool IsTeamFull(int Team_Index) const;
	bool LeaveTeam(APlayerState * Player);


protected:
	void SetMaxTeamSize(int8 InTeamSize);
	void SetNumTeams(int8 InNumTeams);

protected:
	virtual void OnRep_ReplicatedHasBegunPlay() override;
	virtual void ReceivedGameModeClass() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


protected:
	bool initialized = false;
	TArray<TArray<APlayerState *>> Teams;

	UPROPERTY(Replicated)
	int8 MaxTeamSize = -1;

	UPROPERTY(Replicated)
	int8 NumTeams = -1;

};
