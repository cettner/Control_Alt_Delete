// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMode.h"
#include "DefaultGameState.h"
#include "FPSServerController.h"
#include "GameAssets.h"
#include "Engine.h"
#include "RTSSelectionCamera.h"
#include "CombatCommander.h"

ADefaultMode::ADefaultMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(TEXT(COMBAT_COMMANDER_BP_PATH));
	if (TargetBlueprint.Object)
	{
		DefaultFPSClass = (UClass*)TargetBlueprint.Object->GeneratedClass;
	}

	DefaultRTSClass = ARTSSelectionCamera::StaticClass();
	GameStateClass = ADefaultGameState::StaticClass();
	PlayerControllerClass = AFPSServerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
	PlayerStateClass = ADefaultPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
}

void ADefaultMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ARTSPlayerController * PC =  Cast<ARTSPlayerController>(NewPlayer);

	if (PC)
	{
		PC->FinishPlayerLogin();
	}
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

	// If incoming start is specified, then just use it
	if (!IncomingName.IsEmpty())
	{
		const FName IncomingPlayerStartTag = FName(*IncomingName);
		for (TActorIterator<APlayerStart> It(World); It; ++It)
		{
			APlayerStart* Start = *It;
			if (Start && Start->PlayerStartTag == IncomingPlayerStartTag)
			{
				return Start;
			}
		}
	}

	// Always pick StartSpot at start of match
	if (ShouldSpawnAtStartSpot(Player))
	{
		if (AActor* PlayerStartSpot = Player->StartSpot.Get())
		{
			return PlayerStartSpot;
		}
		else
		{
			UE_LOG(LogGameMode, Error, TEXT("FindPlayerStart: ShouldSpawnAtStartSpot returned true but the Player StartSpot was null."));
		}
	}

	AActor* BestStart = ChoosePlayerStart(Player);
	if (BestStart == nullptr)
	{
		// No player start found
		UE_LOG(LogGameMode, Log, TEXT("FindPlayerStart: PATHS NOT DEFINED or NO PLAYERSTART with positive rating"));

		// This is a bit odd, but there was a complex chunk of code that in the end always resulted in this, so we may as well just 
		// short cut it down to this.  Basically we are saying spawn at 0,0,0 if we didn't find a proper player start
		BestStart = World->GetWorldSettings();
	}

	return BestStart;
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

UClass * ADefaultMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	ARTSPlayerController * PC = Cast<ARTSPlayerController>(InController);
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();
	if (PC && GS)
	{
		ADefaultPlayerState * PlayerState = Cast<ADefaultPlayerState>(PC->PlayerState);

		if (PlayerState)
		{
			if (PlayerState->isRtsPlayer)
			{
				return(DefaultRTSClass);
			}
			else
			{
				return(DefaultFPSClass);
			}
		}
		else
		{
			return(nullptr);
		}

	}
	else
	{
		return(nullptr);
	}
}

