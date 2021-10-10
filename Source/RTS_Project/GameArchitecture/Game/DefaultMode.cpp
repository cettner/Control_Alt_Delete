// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMode.h"
#include "DefaultGameState.h"
#include "Engine.h"
#include "DefaultPlayerState.h"
#include "DefaultHUD.h"



ADefaultMode::ADefaultMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	GameStateClass = ADefaultGameState::StaticClass();
	PlayerStateClass = ADefaultPlayerState::StaticClass();
	HUDClass = ADefaultHUD::StaticClass();
	
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
	const bool allplayersregistered = CheckPlayerRegistry();
	return (allplayersregistered);
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
		const bool bhasplayerregistered = RegisterPlayerData(newcontroller, settings);
		newcontroller->SetIsRegistered(bhasplayerregistered);
		if (!bhasplayerregistered)
		{
			UE_LOG(LogTemp, Warning, TEXT("[DEFAULTGAMEMODE::PostLogin] Listen Server Failed to Register Player Data"));
		}

	}
}

void ADefaultMode::StartMatch()
{
	Super::StartMatch();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ADefaultPlayerController* firstcontroller = World->GetFirstPlayerController<ADefaultPlayerController>();
	if (World->GetNetMode() == NM_ListenServer)
	{
		firstcontroller->OnMatchStart();
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

	#if !WITH_EDITOR
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
	#else
		/*In the editor, Create a fake ID based off the default settings*/
		for (int i = 0; i < LobbyPlayers.Num(); i++)
		{
			LobbyPlayers[i].PlayerId = EditorCreatePlayerID();
			PlayerRegistry.Emplace(LobbyPlayers[i].PlayerId, false);
		}


	#endif

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

	#else
	if (settings.bIsValid == false || RegisteringPlayer == nullptr) return false;

	if (PlayerRegistry.Contains(settings.PlayerId))
	{
		PlayerRegistry[settings.PlayerId] = true;
		retval = PlayerRegistry[settings.PlayerId];
	}
	#endif

	retval &= FinishPlayerRegistration(RegisteringPlayer, settings);
	return(retval);
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


void ADefaultMode::InitializeDeferredDefaultPawn(APawn * DefferedPawn, AController * InheritingController)
{
	/*Virtual Function for Initializing Pawn Prior To Spawn*/
}

APawn * ADefaultMode::SpawnDefaultPawnAtTransform_Implementation(AController * NewPlayer, const FTransform & SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer);

	APawn* ResultPawn = GetWorld()->SpawnActorDeferred<APawn>(PawnClass, SpawnTransform, NewPlayer, GetInstigator());
	InitializeDeferredDefaultPawn(ResultPawn, NewPlayer);
	ResultPawn->FinishSpawning(SpawnTransform);


	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
	}

	return ResultPawn;
}

#if WITH_EDITOR
FPlayerSettings ADefaultMode::EditorFetchPlayerSettings(APlayerController* Controller)
{
	FPlayerSettings retval = FPlayerSettings();
	if (EditorPlayerCount < LobbyPlayers.Num() && Controller != nullptr)
	{
		retval = LobbyPlayers[EditorPlayerCount];
		EditorPlayerCount++;
	}
	return(retval);
}
TSharedPtr<const FUniqueNetId> ADefaultMode::EditorCreatePlayerID()
{
	static uint8 currentseed = 0U;
	FEditorUniqueNetID * fakeid = new FEditorUniqueNetID();
	fakeid->SetSeed(currentseed);
	/*increment so next id will be "unique"*/
	currentseed++;

	TSharedPtr<FEditorUniqueNetID> retval = TSharedPtr<FEditorUniqueNetID>(fakeid);

	return retval;
}
#endif

