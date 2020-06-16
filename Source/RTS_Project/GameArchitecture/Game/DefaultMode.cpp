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

	PlayerRegistry = TMap<int, bool>();
	LobbyPlayers = TArray<FPlayerSettings>();
}

void ADefaultMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UWorld* World = GetWorld();
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();


	/*Create a Set of spawn points for each team*/
	for (int i = 0; i < NumTeams; i++)
	{
		TeamSpawnSelector newteam;
		TeamStartingPoints.Emplace(newteam);
	}
	
	/*Find PlayerStarts For each team and add them*/
	for (TActorIterator<ATeamPlayerStart> It(World); It; ++It)
	{
		ATeamPlayerStart* Start = *It;
		if (GS && GS->IsTeamValid(Start->teamid))
		{
			TeamStartingPoints[Start->teamid].Add(Start);
		}
	}

	if (!LoadServerData())
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::PreInitializeComponents] Failed to Load Server Data from Game Instance"));
	}
}

void ADefaultMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

AActor * ADefaultMode::FindPlayerStart_Implementation(AController * Player, const FString & IncomingName)
{
	UWorld* World = GetWorld();
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(Player->PlayerState);
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	if (PS && GS)
	{
		if (GS->IsTeamValid(PS->Team_ID) && TeamStartingPoints.Num())
		{
			ATeamPlayerStart * retval = TeamStartingPoints[PS->Team_ID].GetNextSpawn();
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

void ADefaultMode::InitGameState()
{
	Super::InitGameState();
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	if (GS)
	{
		GS->TeamInitialize(this);
	}
}

void ADefaultMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	/*IF we're on a listen server, register the servers playercontroller*/
	if (GetNetMode() == NM_ListenServer && NewPlayer == World->GetFirstPlayerController())
	{
		ADefaultPlayerController* PC = World->GetFirstPlayerController<ADefaultPlayerController>();
		FPlayerSettings settings;
		if (PC && PC->GetPlayerInfo(settings))
		{
			RegisterPlayerData(PC, settings);
		}
	}
}

bool ADefaultMode::LoadServerData()
{

	ULobbyGameInstance* GI = GetGameInstance<ULobbyGameInstance>();

	FServerSettings settings = GI->GetServerSettings();

	if (settings.bIsValid == false) return false;
	
	bool retval = true;

	NumTeams = settings.NumTeams;
	TeamSize = settings.NumPlayersPerTeam;
	LobbyPlayers = settings.settings;

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

	PS->Team_ID = LobbyPlayers[playerindex].TeamId;
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


