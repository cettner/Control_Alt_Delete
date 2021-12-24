// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPlayerState.h"
#include "DefaultPlayerController.h"
#include "DefaultGameState.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

void ADefaultPlayerState::ClientInitialize(AController* Controller)
{
	Super::ClientInitialize(Controller);
	ADefaultPlayerController * pc = Cast<ADefaultPlayerController>(Controller);
	check(pc);

	if (pc->IsRegistered())
	{
		pc->PostRegisterInit();
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
	bHasDefaultGameModeDataLoaded = true;
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