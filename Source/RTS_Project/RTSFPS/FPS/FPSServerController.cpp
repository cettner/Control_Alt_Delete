// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSServerController.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"

#include "Net/UnrealNetwork.h"




bool AFPSServerController::Server_Request_Interact_Validate(ACommander * Controlled_Cmdr, AActor * Interacted)
{
		return(true);
}

void AFPSServerController::Server_Request_Interact_Implementation(ACommander * Controlled_Cmdr, AActor * Interacted)
{
	if(Cast<ARTSMinion>(Interacted))
	{
		Controlled_Cmdr->MinionInteractionHandler(Cast<ARTSMinion>(Interacted));
	}
	else if(Cast<IRTSObjectInterface>((Interacted)))
	{
		Controlled_Cmdr->SelectableInterationHandler(Interacted);
	}
	else
	{

	}
}

void AFPSServerController::SelectRespawnStructure(ARTSStructure* RespawnStructure)
{
	if (!HasAuthority())
	{
		ServerSelectRespawnStructure(RespawnStructure);
	}
	else
	{
		if (RespawnStructure->CanSpawn(ACommander::StaticClass()) && !isSpawningMinion)
		{
			isSpawningMinion = RespawnStructure->QueueMinion(ACommander::StaticClass(), this);
		}
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

void AFPSServerController::OnPawnDeath()
{
}

void AFPSServerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	isSpawningMinion = false;
}

bool AFPSServerController::ServerSelectRespawnStructure_Validate(ARTSStructure* SelectedStructure)
{
	return(true);
}

void AFPSServerController::ServerSelectRespawnStructure_Implementation(ARTSStructure* SelectedStructure)
{
	SelectRespawnStructure(SelectedStructure);
}

void AFPSServerController::OnRep_isSpawningMinion()
{

}

void AFPSServerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSServerController, isSpawningMinion);
}

