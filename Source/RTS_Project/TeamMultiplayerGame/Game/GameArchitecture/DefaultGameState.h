// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DefaultPlayerState.h"
#include "DefaultMode.h"
#include "TeamState.h"
#include "DefaultGameState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultGameState : public AGameState
{
	GENERATED_BODY()
	
friend class ATeamState;

public:
	virtual bool TeamInitialize(ADefaultMode * GameMode);
	virtual void PlayerGameDataInit(ADefaultPlayerState * Player);
	ATeamState* AssignTeam(ADefaultPlayerState* Player, const int teamid);
	ATeamState* AssignAvailableTeam(ADefaultPlayerState * New_Player);
	ATeamState* AssignBalancedTeam(ADefaultPlayerState* New_Player);
	bool SwapTeam(ADefaultPlayerState* Player, int New_Team);
	ATeamState * GetTeamForPlayer(ADefaultPlayerState* Player) const;
	bool IsTeamValid(int team_id) const;
	bool IsTeamFull(int Team_Index) const;
	bool LeaveTeam(ADefaultPlayerState* Player);


protected:
	void SetMaxTeamSize(int8 InTeamSize);
	void SetNumTeams(int8 InNumTeams);

protected:
	virtual void OnLocalTeamStateRecieved(ATeamState* InState);

public:
	virtual ATeamState* GetTeamState(const int InTeamId) const;
	
	template<class T>
	T* GetTeamState(const int InTeamID) const
	{
		return Cast<T>(GetTeamState(InTeamID));
	}
	
	virtual ATeamState* GetDefaultTeamState() const;
	
	template<class T>
	T* GetDefaultTeamState() const
	{
		return Cast<T>(GetDefaultTeamState());
	}


protected:
	virtual void ReceivedGameModeClass() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:


protected:
	bool initialized = false;

	TArray<ATeamState*> TeamStates;

protected:
	UPROPERTY(Replicated)
	int8 MaxTeamSize = 0xFFU;

	UPROPERTY(Replicated)
	int8 NumTeams = 0U;

};
