// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameAssets.h"
#include "UnrealNetwork.h"
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

void ARTFPSGameState::BeginPlay()
{
	Super::BeginPlay();
}

AFogOfWarManager * ARTFPSGameState::InitFOW()
{
	FActorSpawnParameters SpawnParams;
	UWorld * World = GetWorld();
	if (World && FOWManagerClass)
	{
		return(World->SpawnActor<AFogOfWarManager>(FOWManagerClass, SpawnParams));
	}

	return(nullptr);
}

void ARTFPSGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ARTFPSGameState, FOWManager, COND_InitialOnly);
}

