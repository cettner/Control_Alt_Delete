// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMode.h"
#include "DefaultGameState.h"
#include "Engine.h"
#include "DefaultPlayerState.h"

ADefaultMode::ADefaultMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	GameStateClass = ADefaultGameState::StaticClass();
	PlayerStateClass = ADefaultPlayerState::StaticClass();
	
	TeamStartingPoints = TArray<TeamSpawnSelector>();
	PlayerRegistry = TMap<int, bool>();
	LobbyPlayers = TArray<FPlayerSettings>();
	NumTeams = -1;
	TeamSize = -1;
}

void ADefaultMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UWorld* World = GetWorld();


	if (!LoadServerData())
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::PreInitializeComponents] Failed to Load Server Data from Game Instance"));
	}

	ADefaultGameState* GS = GetGameState<ADefaultGameState>();
	if (GS == nullptr) return;

	/*Create a Set of spawn points for each team*/
	for (int i = 0; i < NumTeams; i++)
	{
		TeamSpawnSelector newteam;
		TeamStartingPoints.Emplace(newteam);
	}

	GS->TeamInitialize(this);

	/*Find PlayerStarts For each team and add them*/
	for (TActorIterator<ATeamPlayerStart> It(World); It; ++It)
	{
		ATeamPlayerStart* Start = *It;
		if (GS->IsTeamValid(Start->teamid))
		{
			TeamStartingPoints[Start->teamid].Add(Start);
		}
	}
}

AActor * ADefaultMode::FindPlayerStart_Implementation(AController * Player, const FString & IncomingName)
{
	UWorld* World = GetWorld();
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(Player->PlayerState);
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	if (PS && GS)
	{
		if (GS->IsTeamValid(PS->TeamID) && TeamStartingPoints.Num())
		{
			ATeamPlayerStart * retval = TeamStartingPoints[PS->TeamID].GetNextSpawn();
			if (retval)
			{
				return(retval);
			}
		}
	}

	return (ChoosePlayerStart(Player));
}

bool ADefaultMode::ReadyToStartMatch_Implementation()
{
	return CheckPlayerRegistry();
}

void ADefaultMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ADefaultPlayerController* PC = World->GetFirstPlayerController<ADefaultPlayerController>();

	/*IF we're on a listen server, register the servers playercontroller and */
	if (GetNetMode() == NM_ListenServer && NewPlayer == World->GetFirstPlayerController())
	{
		FPlayerSettings settings;
		if (PC && PC->GetPlayerInfo(settings))
		{
			if (RegisterPlayerData(PC, settings))
			{
				PC->ClientNotifyTeamChange(PC->GetPlayerState<ADefaultPlayerState>()->TeamID);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::PostLogin] Listen Server Failed to Register Player Data"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::PostLogin] Listen Server Invalid Player Data"));
		}
	}
}

bool ADefaultMode::LoadServerData()
{
	ULobbyGameInstance* GI = GetGameInstance<ULobbyGameInstance>();
	if (GI == nullptr) return(false);

	FServerSettings settings = GI->GetServerSettings();

	if (settings.bIsValid == false) return false;
	
	bool retval = true;

	NumTeams = settings.NumTeams;
	TeamSize = settings.NumPlayersPerTeam;
	LobbyPlayers = settings.settings;

	/*Empty the Registry*/
	PlayerRegistry.Empty();

	for (int i = 0; i < LobbyPlayers.Num(); i++)
	{
		if (LobbyPlayers[i].bIsValid  && !PlayerRegistry.Contains(LobbyPlayers[i].PlayerId))
		{
			PlayerRegistry.Emplace(LobbyPlayers[i].PlayerId, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::LoadServerData] Server Player Settings are Invalid"));
			retval = false;
		}
	}

	return retval;
}

bool ADefaultMode::RegisterPlayerData(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings)
{
	if (settings.bIsValid == false || RegisteringPlayer == nullptr) return false;

	if (PlayerRegistry.Contains(settings.PlayerId))
	{
		PlayerRegistry[settings.PlayerId] = FinishPlayerRegistration(RegisteringPlayer, settings);
		return(PlayerRegistry[settings.PlayerId]);
	}
	else
	{
		/*TODO:: Implement adding of player midgame*/
	}

	return false;
}

bool ADefaultMode::FinishPlayerRegistration(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings)
{
	ADefaultPlayerState* PS = RegisteringPlayer->GetPlayerState<ADefaultPlayerState>();
	int playerindex = INDEX_NONE; 
	
	for (int i = 0; i < LobbyPlayers.Num(); i++)
	{
		if (LobbyPlayers[i].PlayerId == settings.PlayerId)
		{
			playerindex = i;
			break;
		}
	}
	
	if (playerindex <= INDEX_NONE || PS == nullptr) return(false);

	PS->TeamID = LobbyPlayers[playerindex].TeamId;
	PS->PlayerId = LobbyPlayers[playerindex].PlayerId;

	return true;
}

bool ADefaultMode::CheckPlayerRegistry()
{
	bool ballplayersregistered = (PlayerRegistry.Num() > 0);

	for (TPair<int,bool>& Elem : PlayerRegistry)
	{
		ballplayersregistered &= Elem.Value;
	}

	return(ballplayersregistered);
}


