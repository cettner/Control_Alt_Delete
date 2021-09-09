// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "DefaultMode.h"
#include "DefaultPlayerState.h"
#include "DefaultHUD.h"

#include "Net/UnrealNetwork.h"


bool ADefaultPlayerController::ServerRegisterPlayerInfo_Validate(FPlayerSettings settings)
{
	return(true);
}

void ADefaultPlayerController::ServerRegisterPlayerInfo_Implementation(FPlayerSettings settings)
{
		RegisterPlayerInfo(settings);
}

void ADefaultPlayerController::ClientRequestRegistration_Implementation()
{
	RequestRegistration();
}

void ADefaultPlayerController::ClientNotifyTeamChange(int newteamid)
{
	/*Pure Virtual Function*/
}

int ADefaultPlayerController::GetTeamID() const
{
	ADefaultPlayerState * PS = GetPlayerState<ADefaultPlayerState>();
	if (PS)
	{
		return(PS->TeamID);
	}
	return -1;
}

void ADefaultPlayerController::OnMatchStart()
{
}

void ADefaultPlayerController::SetIsRegistered(bool bregistered)
{
	bisregistered = bregistered;

	if ((GetWorld()->GetFirstPlayerController() == this) && HasAuthority())
	{
		OnRep_bisregistered();
	}
}

void ADefaultPlayerController::RegisterPlayerInfo(FPlayerSettings settings)
{
	if (HasAuthority() && (bisregistered == false))
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		ADefaultMode * GM = World->GetAuthGameMode<ADefaultMode>();
		if (GM == nullptr) return;

		bisregistered = GM->RegisterPlayerData(this,settings);

		/*Inititalize the local server*/
		if (bisregistered && (World->GetFirstPlayerController() == this) && (World->GetNetMode() == NM_ListenServer))
		{
			PostRegisterInit();
		}
	}
	else
	{
		ServerRegisterPlayerInfo(settings);
	}
}

void ADefaultPlayerController::RequestRegistration()
{
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		FPlayerSettings mysettings;
		if (!GetPlayerInfo(mysettings)) /*NOT WORKING IN EDITOR BECAUSE DATA IS INVALID*/
		{
			UE_LOG(LogTemp, Warning, TEXT("[DefaultPlayerController::RequestRegistration] Failed to get player settings"));
		}

		ServerRegisterPlayerInfo(mysettings);
	}
}

void ADefaultPlayerController::OnRep_bisregistered()
{
	/*If Registration happens after Client Initialization or we're the server, finish the local player setup otherwise wait for playerstate to initialize*/
	/*Warning: On Client Player State may not even be created yet.*/

	ADefaultPlayerState * ps = GetPlayerState<ADefaultPlayerState>();
	if (IsValid(ps) && ps->IsClientInitialized() && bisregistered == true)
	{
		PostRegisterInit();
	}
}

void ADefaultPlayerController::FinishLocalPlayerSetup()
{
	/*Virtual Function Child Implementation Goes Here*/
}

void ADefaultPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADefaultPlayerController, bisregistered);
}

void ADefaultPlayerController::PostRegisterInit()
{
	/*Client Will inititialize its UI After Gamestate's initial Replication, Server can Start Here*/
	if ((GetWorld()->GetNetMode() == NM_ListenServer) && (GetWorld()->GetFirstPlayerController() == this) && HasAuthority())
	{
		ClientInitUI();
	}

	FinishLocalPlayerSetup();
}

void ADefaultPlayerController::ClientInitUI()
{
	if (IsRegistered() && IsLocalController())
	{
		ADefaultHUD * hud = GetHUD<ADefaultHUD>();
		hud->InitializeUI();
	}
}

bool ADefaultPlayerController::GetPlayerInfo(FPlayerSettings& outsettings)
{
	ULobbyGameInstance * GI= GetGameInstance<ULobbyGameInstance>();
	if (GI == nullptr) return false;

	outsettings = GI->GetPlayerSettings();

	return (outsettings.bIsValid);
}
