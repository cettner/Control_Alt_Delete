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
	PlayerRegistry = TMap<TSharedPtr<const FUniqueNetId>, bool>();
	LobbyPlayers = TArray<FPlayerSettings>();
	NumTeams = -1;
	TeamSize = -1;

	DefaultSettings.bIsValid = true;
	DefaultSettings.NumPlayersPerTeam = 2;
	DefaultSettings.NumTeams = 2;
}

void ADefaultMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UWorld* World = GetWorld();

	/*Load Lobby Data from GameInstance*/
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

	ADefaultPlayerController* firstcontroller = World->GetFirstPlayerController<ADefaultPlayerController>();
	ADefaultPlayerController* newcontroller = Cast<ADefaultPlayerController>(NewPlayer);

	const ENetMode netmode = GetNetMode();
	const bool bnetmodevalid = (netmode == NM_ListenServer || netmode == NM_Standalone);
	
	const EWorldType::Type worldtype = World->WorldType;
	FPlayerSettings settings = FetchSettingsFromLobbyData(NewPlayer);

	if (!settings.bIsValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::PostLogin] Listen Server Failed to Find Player Data"));
		return;
	}
	else
	{
		FinishPlayerRegistration(newcontroller, settings);
		if (!RegisterPlayerData(newcontroller, settings))
		{
			UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::PostLogin] Listen Server Failed to Register Player Data"));
		}
	}

	/*If we're on a listen server, register the servers playercontroller and play local effects*/
	if ((bnetmodevalid == true) && (NewPlayer == World->GetFirstPlayerController()))
	{
		firstcontroller->ClientNotifyTeamChange(firstcontroller->GetPlayerState<ADefaultPlayerState>()->TeamID);
	}
}

bool ADefaultMode::LoadServerData()
{
	ULobbyGameInstance* GI = GetGameInstance<ULobbyGameInstance>();
	if (GI == nullptr) return(false);

	FServerSettings settings = GI->GetServerSettings();

	if (settings.bIsValid == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::LoadServerData] Failed to get valid data from GameInstance Using Default."));
		settings = GetDefaultSettings();
	}
	
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
	bool retval = false;

	#if WITH_EDITOR
	/*In Editor*/
	for (TPair<TSharedPtr<const FUniqueNetId>, bool>& Elem : PlayerRegistry)
	{
		if (Elem.Value == false)
		{
			PlayerRegistry[Elem.Key] = true;
			retval = true;
			break;
		}
	}

	return retval;
	#else
	if (settings.bIsValid == false || RegisteringPlayer == nullptr) return false;

	if (PlayerRegistry.Contains(settings.PlayerId))
	{
		PlayerRegistry[settings.PlayerId] = true;
		return(PlayerRegistry[settings.PlayerId]);
	}
	return false;
	#endif
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

	return true;
}

FPlayerSettings ADefaultMode::FetchSettingsFromLobbyData(APlayerController* NewPlayer)
{
#if WITH_EDITOR
	return(EditorFetchPlayerSettings(NewPlayer));
#else

	const ULobbyGameInstance * gi = GetGameInstance<ULobbyGameInstance>();
	FPlayerSettings retval = FPlayerSettings();
	const FUniqueNetIdRepl registeringid = ULobbyGameInstance::GetUniquePlayerNetId(NewPlayer);
	
	for (int i = 0; i < LobbyPlayers.Num(); i++)
	{
		if (LobbyPlayers[i].PlayerId == registeringid)
		{
			retval = LobbyPlayers[i];
		}
	}

	return(retval);
#endif
}

bool ADefaultMode::CheckPlayerRegistry()
{
	bool ballplayersregistered = (PlayerRegistry.Num() > 0);

	for (TPair<TSharedPtr<const FUniqueNetId>,bool>& Elem : PlayerRegistry)
	{
		ballplayersregistered &= Elem.Value;
	}

	return(ballplayersregistered);
}

FServerSettings ADefaultMode::GetDefaultSettings() const
{
	return DefaultSettings;
}

#if WITH_EDITOR
FPlayerSettings ADefaultMode::EditorFetchPlayerSettings(APlayerController* Controller)
{
	FPlayerSettings retval = FPlayerSettings();
	if (EditorPlayerCount < LobbyPlayers.Num() && Controller != nullptr)
	{
		LobbyPlayers[EditorPlayerCount].PlayerId = ULobbyGameInstance::GetUniquePlayerNetId(Controller).GetUniqueNetId();
		retval = LobbyPlayers[EditorPlayerCount];
		EditorPlayerCount++;
	}
	return(retval);
}
#endif

