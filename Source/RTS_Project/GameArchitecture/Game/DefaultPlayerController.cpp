// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "DefaultMode.h"
#include "DefaultPlayerState.h"

void ADefaultPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

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

void ADefaultPlayerController::RegisterPlayerInfo(FPlayerSettings settings)
{
	if (HasAuthority() && (bisregistered == false))
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		ADefaultMode * GM = World->GetAuthGameMode<ADefaultMode>();
		if (GM == nullptr) return;

		bisregistered = GM->RegisterPlayerData(this,settings);
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
		if (GetPlayerInfo(mysettings))
		{
			ServerRegisterPlayerInfo(mysettings);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[DefaultPlayerController::RequestRegistration] Failed to get player settings"));
		}
	}
}

bool ADefaultPlayerController::GetPlayerInfo(FPlayerSettings& outsettings)
{
	ULobbyGameInstance * GI= GetGameInstance<ULobbyGameInstance>();
	if (GI == nullptr) return false;

	outsettings = GI->GetPlayerSettings();

	return (outsettings.bIsValid);
}
