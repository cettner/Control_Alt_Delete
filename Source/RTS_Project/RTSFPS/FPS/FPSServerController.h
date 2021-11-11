// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "../BaseClasses/RTSPlayerController.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"
#include "Commander.h"
#include "FPSServerController.generated.h"

class ARTSStructure;


UCLASS()
class RTS_PROJECT_API AFPSServerController : public ARTSPlayerController, public IUpgradableInterface
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(Server, reliable, WithValidation)
		void Server_Request_Interact(ACommander * Controlled_Cmdr, AActor * Interacted);

		void SelectRespawnStructure(ARTSStructure* SelectedStructure);
		
		virtual void ClientNotifyTeamChange(int newteamid) override;

		virtual void OnPawnDeath();


    protected:
		virtual void OnPossess(APawn* InPawn) override;

    protected:

		UFUNCTION(Server, reliable, WithValidation)
		void ServerSelectRespawnStructure(ARTSStructure* SelectedStructure);

    protected:
	    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
