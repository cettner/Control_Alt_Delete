// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMode.h"
#include "DefaultGameState.h"
#include "Engine.h"

#include "DefaultPlayerState.h"

ADefaultMode::ADefaultMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	GameStateClass = ADefaultGameState::StaticClass();
	PlayerStateClass = ADefaultPlayerState::StaticClass();

	for (int i = 0; i < num_teams; i++)
	{
		TeamSpawnSelector newteam;
		TeamStartingPoints.Emplace(newteam);
	}
}

void ADefaultMode::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	for (TActorIterator<ATeamPlayerStart> It(World); It; ++It)
	{
		ATeamPlayerStart* Start = *It;
		if (GS && GS->IsTeamValid(Start->teamid))
		{
			TeamStartingPoints[Start->teamid].Add(Start);
		}
	}
}

void ADefaultMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ADefaultMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
}

AActor * ADefaultMode::FindPlayerStart_Implementation(AController * Player, const FString & IncomingName)
{
	if (Cast<APlayerStart>(Player->StartSpot))
	{
		return(Cast<APlayerStart>(Player->StartSpot));
	}

	UWorld* World = GetWorld();
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(Player->PlayerState);
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	if (PS && GS)
	{
		if (GS->IsTeamValid(PS->Team_ID) && TeamStartingPoints.Num())
		{
			ATeamPlayerStart * retval = TeamStartingPoints[PS->Team_ID].GetNextSpawn();
			if (retval)
			{
				return(retval);
			}
		}
	}

	return (ChoosePlayerStart(Player));
}

void ADefaultMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ADefaultMode::InitGameState()
{
	Super::InitGameState();
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	if (GS)
	{
		GS->TeamInitialize(this);
	}
}


