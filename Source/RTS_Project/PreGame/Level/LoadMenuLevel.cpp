// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadMenuLevel.h"
#include "RTS_Project\LobbySystem\GameArchitecture\Instance\LobbyGameInstance.h"


void ALoadMenuLevel::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	ULobbyGameInstance * gi = GetGameInstance<ULobbyGameInstance>();
	if (IsValid(gi))
	{

	}
}

#ifdef WITH_EDITOR
void ALoadMenuLevel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ULobbyGameInstance* gi = GetGameInstance<ULobbyGameInstance>();
	if (IsValid(gi))
	{

	}
}
#endif // WITH_EDITOR


