// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSServerController.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSFPSHUD.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSFPSWidget.h"
#include "RTS_Project/RTSFPS/FPS/UI/FPSUI.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"
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
	if (!HasAuthority())
	{
		ServerPurchaseExpUpgrade(UpgradeClass);
	}
	else
	{
		const UWorld * world = GetWorld();
		const ARTFPSGameState * gs = world->GetGameState<ARTFPSGameState>();
		AFPSPlayerState * ps = GetPlayerState<AFPSPlayerState>();
		gs->PurchaseExpUpgrade(UpgradeClass, ps, ps);
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
	GetPlayerState<AFPSPlayerState>()->SetRespawnState(EPlayerReswpawnState::SELECTINGRESPAWN);
}

void AFPSServerController::ToggleUpgradeMenu()
{
	const ARTSFPSHUD* hud = GetHUD<ARTSFPSHUD>();
	if (hud == nullptr) return;

	const URTSFPSWidget * mainui = hud->GetPrimaryUI<URTSFPSWidget>();
	if (mainui == nullptr) return;

	UFPSUI * fpsui = mainui->GetCurrentUI<UFPSUI>();
	
	/*If we're in FPS mode, this wont return null*/
	if (fpsui != nullptr && !bisUpgradeMenuOpen)
	{
		fpsui->ShouldShowUpgradeTree(true);
		FInputModeGameAndUI inputmode;
		SetInputMode(inputmode);
		bShowMouseCursor = true;

		bisUpgradeMenuOpen = true;
	}
	else if (fpsui != nullptr && bisUpgradeMenuOpen)
	{
		fpsui->ShouldShowUpgradeTree(false);
		FInputModeGameOnly inputMode;
		inputMode.SetConsumeCaptureMouseDown(false);
		SetInputMode(inputMode);
		bShowMouseCursor = false;

		bisUpgradeMenuOpen = false;
	}
}

void AFPSServerController::RefreshUpgradeMenu()
{
	const ARTSFPSHUD* hud = GetHUD<ARTSFPSHUD>();
	if (hud == nullptr) return;

	const URTSFPSWidget * mainui = hud->GetPrimaryUI<URTSFPSWidget>();
	if (mainui == nullptr) return;

	UFPSUI * fpsui = mainui->GetCurrentUI<UFPSUI>();

	if (fpsui != nullptr)
	{
		fpsui->RefreshUpgradeTree();
	}

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
	InputComponent->BindAction("BKey", IE_Pressed, this, &AFPSServerController::GrantPlayerExp);
}



/**************************************Debug / Testing*******************************************/
bool AFPSServerController::ServerGrantPlayerExp_Validate()
{
	return true;
}

void AFPSServerController::ServerGrantPlayerExp_Implementation()
{
	GrantPlayerExp();
}

void AFPSServerController::GrantPlayerExp()
{
	if (!HasAuthority())
	{
		ServerGrantPlayerExp();
	}
	else
	{
		AFPSPlayerState * ps = GetPlayerState<AFPSPlayerState>();
		ps->GrantExp(ExptoGrant);
	}
}
