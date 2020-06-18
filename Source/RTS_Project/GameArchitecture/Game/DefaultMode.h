// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RTSFPS/GameObjects/TeamPlayerStart.h"
#include "GameArchitecture/Game/DefaultPlayerController.h"
#include "GameArchitecture/Instance/LobbyGameInstance.h"
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
	virtual bool RegisterPlayerData(ADefaultPlayerController * RegisteringPlayer, FPlayerSettings settings);
	int GetNumTeams() { return (NumTeams); }
	int GetTeamSize() { return (TeamSize); }

protected:
	virtual void PostInitializeComponents() override;
	virtual AActor * FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual bool LoadServerData();
	virtual bool FinishPlayerRegistration(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings);
	virtual bool CheckPlayerRegistry();

protected:
/************************************/
	/*Loaded from Game Instance*/
	int NumTeams = 2;
	int TeamSize = 2;
	TArray<FPlayerSettings> LobbyPlayers;
/************************************/
	
	TMap<int, bool> PlayerRegistry;
	TArray<TeamSpawnSelector> TeamStartingPoints;
};
