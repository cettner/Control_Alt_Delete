// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RTSFPS/GameObjects/TeamPlayerStart.h"
#include "DefaultMode.generated.h"

/**
 * 
 */

/*Helper Struct*/
struct TeamSpawnSelector
{
public:
	void Add(ATeamPlayerStart * Add) 
	{
		if (Add && LastSpawn < 0)
		{
			LastSpawn = 0;
		}
		TeamStart.AddUnique(Add);
	}
	ATeamPlayerStart * GetNextSpawn()
	{
		ATeamPlayerStart * retval = nullptr;
		if (TeamStart.Num())
		{
			retval = TeamStart[LastSpawn];
			if (LastSpawn + 1 == TeamStart.Num())
			{
				LastSpawn = 0;
			}
			else
			{
				LastSpawn++;
			}

		}
		return(retval);
	}
	int Num() 
	{
		return(TeamStart.Num());
	}

protected:
	int LastSpawn = -1;
	TArray<ATeamPlayerStart *> TeamStart;
};

UCLASS()
class RTS_PROJECT_API ADefaultMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADefaultMode(const FObjectInitializer& ObjectInitializer);
	int GetNumTeams() { return (num_teams); }
	int GetTeamSize() { return(team_size);  }

protected:
	virtual void PostInitializeComponents() override;
	virtual AActor * FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	virtual bool ReadyToStartMatch_Implementation();
	virtual void InitGameState() override;

protected:
	int num_teams = 2;
	int team_size = 2;
	TArray<TeamSpawnSelector> TeamStartingPoints;
};
