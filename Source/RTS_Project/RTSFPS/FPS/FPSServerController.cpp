// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSServerController.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTSFPSHUD.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTSFPSWidget.h"
#include "RTS_Project/RTSFPS/FPS/UI/FPSUI.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"
#include "FPSPlayerState.h"

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
		EPlayerReswpawnState currentstate = GetPlayerState<AFPSPlayerState>()->GetRespawnState();

		if (RespawnStructure->CanSpawn(ACommander::StaticClass()) && (currentstate == EPlayerReswpawnState::SELECTINGRESPAWN))
		{
			 const bool queuecheck = RespawnStructure->QueueActor(ACommander::StaticClass(), this);
			 check(queuecheck);
			 GetPlayerState<AFPSPlayerState>()->SetRespawnState(EPlayerReswpawnState::AWAITINGRESPAWN);

		}
	}
}

void AFPSServerController::PurchaseExpUpgrade(TSubclassOf<UUpgrade> UpgradeClass)
{
	AFPSPlayerState* ps = GetPlayerState<AFPSPlayerState>();
	const bool canupgrade = ps->MeetsUpgradeDependencies(UpgradeClass);
	if (canupgrade)
	{
		if (!HasAuthority())
		{
			ServerPurchaseExpUpgrade(UpgradeClass);
		}
		else
		{
			const UWorld* world = GetWorld();
			const ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
			gs->PurchaseExpUpgrade(UpgradeClass, ps, ps);
		}
	}
}

void AFPSServerController::OnPawnDeath()
{
	GetPlayerState<AFPSPlayerState>()->SetRespawnState(EPlayerReswpawnState::SELECTINGRESPAWN);
}

void AFPSServerController::ToggleUpgradeMenu()
{
	ARTSFPSHUD* hud = GetHUD<ARTSFPSHUD>();
	if (hud == nullptr) return;

	hud->TryToggleUpgradeTree();
}

void AFPSServerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Cast<ACommander>(InPawn))
	{
		GetPlayerState<AFPSPlayerState>()->SetRespawnState(EPlayerReswpawnState::ALIVE);
	}


}

bool AFPSServerController::ServerSelectRespawnStructure_Validate(ARTSStructure* SelectedStructure)
{
	return(true);
}

void AFPSServerController::ServerSelectRespawnStructure_Implementation(ARTSStructure* SelectedStructure)
{
	SelectRespawnStructure(SelectedStructure);
}

bool AFPSServerController::ServerPurchaseExpUpgrade_Validate(TSubclassOf<UUpgrade> UpgradeClass)
{
	return true;
}

void AFPSServerController::ServerPurchaseExpUpgrade_Implementation(TSubclassOf<UUpgrade> UpgradeClass)
{
	PurchaseExpUpgrade(UpgradeClass);
}

void AFPSServerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("NKey", IE_Pressed, this, &AFPSServerController::ToggleUpgradeMenu);
}

