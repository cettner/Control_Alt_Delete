// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "../RTS/RTSPlayerController.h"
#include "Commander.h"
#include "FPSServerController.generated.h"

class ARTSStructure;


UCLASS()
class RTS_PROJECT_API AFPSServerController : public ARTSPlayerController
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(Server, reliable, WithValidation)
		void Server_Request_Interact(ACommander * Controlled_Cmdr, AActor * Interacted);

		void SelectRespawnStructure(ARTSStructure* SelectedStructure);
		
		void PurchaseExpUpgrade(TSubclassOf<UUpgrade> UpgradeClass);

		virtual void OnPawnDeath();

		UFUNCTION()
		virtual void ToggleUpgradeMenu();

    protected:
		virtual void OnPossess(APawn* InPawn) override;

    protected:
		UFUNCTION(Server, reliable, WithValidation)
		void ServerSelectRespawnStructure(ARTSStructure* SelectedStructure);

		UFUNCTION(Server, reliable, WithValidation)
		void ServerPurchaseExpUpgrade(TSubclassOf<UUpgrade> UpgradeClass);

    protected:
		virtual void SetupInputComponent() override;

	protected:
		bool bisUpgradeMenuOpen = false;
		
};
