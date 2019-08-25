// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameAssets.h"
#include "Engine.h"

ARTFPSGameState::ARTFPSGameState(const FObjectInitializer &FOI) : Super(FOI)
{
	ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(TEXT(FOW_MANAGER_PATH));
	if (TargetBlueprint.Object && HasAuthority())
	{
		FOWManagerClass = (UClass*)TargetBlueprint.Object->GeneratedClass;
	}
}

int ARTFPSGameState::NumRTSPlayers(int Team_Index)
{
	int retval = 0;
	if(IsTeamValid(Team_Index))
	{
		for (int i = 0; i < Teams[Team_Index].Num(); i++)
		{
			ARTFPSPlayerState * PS = Cast<ARTFPSPlayerState>(Teams[Team_Index][i]);
			if (PS && PS->isRtsPlayer)
			{
				retval++;
			}
		}
	}
	return(retval);
}

