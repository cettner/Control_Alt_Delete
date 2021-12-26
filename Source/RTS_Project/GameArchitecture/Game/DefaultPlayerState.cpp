// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPlayerState.h"
#include "DefaultPlayerController.h"
#include "DefaultGameState.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"


void ADefaultPlayerState::ClientInitialize(AController* Controller)
{
	Super::ClientInitialize(Controller);

	if (Controller->IsLocalPlayerController())
	{
		SetLocalPlayerState(true);
	}


	ADefaultPlayerController * pc = Cast<ADefaultPlayerController>(Controller);
	check(pc);

	if (pc->IsRegistered())
	{
		pc->PostRegisterInit();
	}

	/*Try to Iniitalize GameModePresets on the client, if it fails just wait for the gamestate to do it for us*/
	if (!bHasDefaultGameModeDataLoaded)
	{
		const UWorld* world = GetWorld();
		const AGameStateBase * gs = world->GetGameState();
		if (gs != nullptr)
		{
			LoadGameModeDefaults(gs->GetDefaultGameMode());
		}
	}

	bisClientInitialized = true;
}

ADefaultPlayerState::ADefaultPlayerState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TeamID = -1;
}

bool ADefaultPlayerState::IsClientInitialized()
{
	if (!HasAuthority())
	{
		/*check if initial Client Data has been sent*/
		return bisClientInitialized;
	}
	else
	{
		/*If we're the server, all data is already present...*/
		return true;
	}
}

bool ADefaultPlayerState::HasDefaultGameModeLoaded()
{
	return bHasDefaultGameModeDataLoaded;
}

void ADefaultPlayerState::LoadGameModeDefaults(const AGameModeBase * GameModeCDO)
{
	/*WARNING: GAMEMODE CDO CANNOT BE MODIFIED*/
	checkf(GameModeCDO,  TEXT("ADefaultPlayerState::LoadGameModeDefaults Default GameMode CDO was NULL"))
	bHasDefaultGameModeDataLoaded = true;
}

bool ADefaultPlayerState::IsLocalPlayerState() const
{
	return bisLocalPlayerState;
}

void ADefaultPlayerState::SetLocalPlayerState(bool inLocalState)
{
	bisLocalPlayerState = inLocalState;

	if (HasAuthority() && (bisLocalPlayerState == true))
	{
		/*Try to Iniitalize GameModePresets on the Server, if it fails just wait for the gamestate to do it for us*/
		if (!bHasDefaultGameModeDataLoaded)
		{
			const UWorld* world = GetWorld();
			const AGameStateBase * gs = world->GetGameState();
			if (gs != nullptr)
			{
				LoadGameModeDefaults(gs->GetDefaultGameMode());
			}
		}
	}
}

void ADefaultPlayerState::OnRep_TeamID()
{
	/*Skip Replication actions if we havnt set up locally yet.*/
	if (!IsClientInitialized()) return;

	UWorld* World =  GetWorld();
	check(World);

	ADefaultPlayerController * pc = World->GetFirstPlayerController<ADefaultPlayerController>();
	check(pc)

	pc->ClientNotifyTeamChange(TeamID);
}

void ADefaultPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADefaultPlayerState, TeamID);
}