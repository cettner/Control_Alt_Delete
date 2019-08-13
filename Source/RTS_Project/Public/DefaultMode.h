// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TeamPlayerStart.h"
#include "DefaultMode.generated.h"

/**
 * 
 */

/*Helper Struct*/
struct TeamSpawnSelector
{
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

private:
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
	virtual bool IsValidTeam(AActor * TeamMember);

public:


protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController * NewPlayer) override;
	virtual void Logout(AController * Exiting) override;
	virtual AActor * ChoosePlayerStart_Implementation(AController* Player) override;
	virtual AActor * FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	virtual void InitGame(const FString & MapName,const FString & Options, FString & ErrorMessage) override;
	virtual void InitGameState() override;

private:
	int num_teams = 2;
	int team_size = 2;
	TArray<TeamSpawnSelector> TeamStartingPoints;
};
