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

	for (int i = 0; i < num_teams; i++)
	{
		TeamSpawnSelector newteam;
		TeamStartingPoints.Emplace(newteam);
	}
}

bool ADefaultMode::IsValidTeam(AActor * TeamMember)
{
	int teamnum = -1;

	if (Cast<ADefaultPlayerState>(TeamMember))
	{
		teamnum = Cast<ADefaultPlayerState>(TeamMember)->Team_ID;
	}
	else if (Cast<ATeamPlayerStart>(TeamMember))
	{
		teamnum = Cast<ATeamPlayerStart>(TeamMember)->teamid;
	}

	return ((teamnum > -1 && teamnum < num_teams));
}

void ADefaultMode::BeginPlay()
{
	UWorld* World = GetWorld();
	for (TActorIterator<ATeamPlayerStart> It(World); It; ++It)
	{
		ATeamPlayerStart* Start = *It;
		if (IsValidTeam(Start))
		{
			TeamStartingPoints[Start->teamid].Add(Start);
		}
	}
}

void ADefaultMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ADefaultMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
}

AActor * ADefaultMode::ChoosePlayerStart_Implementation(AController * Player)
{
	// Choose a player start
	APlayerStart* FoundPlayerStart = nullptr;
	UClass* PawnClass = GetDefaultPawnClassForController(Player);
	APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> UnOccupiedStartPoints;
	TArray<APlayerStart*> OccupiedStartPoints;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (PlayerStart->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			FoundPlayerStart = PlayerStart;
			break;
		}
		else
		{
			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!GetWorld()->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				UnOccupiedStartPoints.Add(PlayerStart);
			}
			else if (GetWorld()->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPoints.Add(PlayerStart);
			}
		}
	}
	if (FoundPlayerStart == nullptr)
	{
		if (UnOccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}
	return FoundPlayerStart;
}

AActor * ADefaultMode::FindPlayerStart_Implementation(AController * Player, const FString & IncomingName)
{
	UWorld* World = GetWorld();
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(Player->PlayerState);
	if (PS)
	{
		if (IsValidTeam(PS) && TeamStartingPoints.Num())
		{
			ATeamPlayerStart * retval = TeamStartingPoints[PS->Team_ID].GetNextSpawn();
			if (retval)
			{
				return(retval);
			}
		}
	}

	return (Super::FindPlayerStart_Implementation(Player,IncomingName));
}

void ADefaultMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
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


