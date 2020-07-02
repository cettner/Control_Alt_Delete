// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSServerController.h"

bool AFPSServerController::Server_Request_Interact_Validate(ACommander * Controlled_Cmdr, AActor * Interacted)
{
	if (Controlled_Cmdr && Interacted)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

void AFPSServerController::Server_Request_Interact_Implementation(ACommander * Controlled_Cmdr, AActor * Interacted)
{
	if(Cast<ARTSMinion>(Interacted))
	{
		Controlled_Cmdr->MinionInteractionHandler(Cast<ARTSMinion>(Interacted));
	}
	else if(Cast<ARTSSelectable>(Interacted))
	{
		Controlled_Cmdr->SelectableInterationHandler(Cast<ARTSSelectable>(Interacted));
	}
	else
	{

	}
}

void AFPSServerController::ClientNotifyTeamChange(int newteamid)
{
	Super::ClientNotifyTeamChange(newteamid);
	/*Listen Server dosn't use replication from playerstate so an initial call is made here at game start*/
	if ((GetLocalRole() == ROLE_Authority && GetRemoteRole() < ROLE_AutonomousProxy))
	{
		ARTSMinion* PlayerPawn = GetPawn<ARTSMinion>();
		if (PlayerPawn == nullptr) return;
		PlayerPawn->SetTeam(newteamid);
	}
}
