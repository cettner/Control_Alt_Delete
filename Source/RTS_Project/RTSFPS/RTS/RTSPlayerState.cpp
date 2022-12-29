// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPlayerState.h"
#include "../GameArchitecture/RTFPSGameState.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


void ARTSPlayerState::InitEnemySelection()
{
	/*Gamestate can be missed due to replication syncing, so we use the "slow" method for getting interface actors*/
	TArray<AActor*> rtsactors = TArray<AActor*>();
	const UWorld* world = GetWorld();
	UGameplayStatics::GetAllActorsWithInterface(world, URTSObjectInterface::StaticClass(), rtsactors);

	for (int i = 0; i < rtsactors.Num(); i++)
	{
		if (IsValid(rtsactors[i]))
		{
			Cast<IRTSObjectInterface>(rtsactors[i])->OnLocalPlayerTeamChange(TeamID);
		}
	}


}