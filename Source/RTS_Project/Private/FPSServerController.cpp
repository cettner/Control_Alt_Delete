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