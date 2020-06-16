// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "GameArchitecture/Game/DefaultMode.h"


void ADefaultPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FPlayerSettings mysettings;
	if (!HasAuthority() && GetPlayerInfo(mysettings))
	{
		RegisterPlayerInfo(mysettings);
	}
	else if(!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[DefaultPlayerController::PostInitializeComponents] Failed to get player settings"));
	}
}

bool ADefaultPlayerController::ServerRegisterPlayerInfo_Validate(FPlayerSettings settings)
{
	return(true);
}

void ADefaultPlayerController::ServerRegisterPlayerInfo_Implementation(FPlayerSettings settings)
{

}

void ADefaultPlayerController::RegisterPlayerInfo(FPlayerSettings settings)
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		ADefaultMode * GM = World->GetAuthGameMode<ADefaultMode>();
		if (GM == nullptr) return;

		GM->RegisterPlayerData(this,settings);
	}
	else
	{
		ServerRegisterPlayerInfo(settings);
	}
}

bool ADefaultPlayerController::GetPlayerInfo(FPlayerSettings& outsettings)
{
	ULobbyGameInstance * GI= GetGameInstance<ULobbyGameInstance>();
	if (GI == nullptr) return false;

	outsettings = GI->GetPlayerSettings();

	return (outsettings.bIsValid);
}
