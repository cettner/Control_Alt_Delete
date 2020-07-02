// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSFPS/BaseClasses/RTSPlayerController.h"
#include "RTSFPS/FPS/Commander.h"
#include "FPSServerController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AFPSServerController : public ARTSPlayerController
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(Server, reliable, WithValidation)
		void Server_Request_Interact(ACommander * Controlled_Cmdr, AActor * Interacted);
	
		virtual void ClientNotifyTeamChange(int newteamid) override;
};
