// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include "DefaultMode.h"
#include "DefaultPlayerState.h"
#include "DefaultHUD.h"

#include "Net/UnrealNetwork.h"



int ADefaultPlayerController::GetTeamID() const
{
	const ADefaultPlayerState * PS = GetPlayerState<ADefaultPlayerState>();
	if (PS)
	{
		return(PS->GetTeamID());
	}
	return -1;
}

void ADefaultPlayerController::SetIsRegistered(bool bregistered)
{
	bisregistered = bregistered;

	if (IsLocalPlayerController() && HasAuthority())
	{
		OnRep_bisregistered();
	}
}

void ADefaultPlayerController::OnRep_bisregistered()
{
	/*If Registration happens after Client Initialization or we're the server, finish the local player setup otherwise wait for playerstate to initialize*/
	/*Warning: On Client Player State may not even be created yet.*/

	const ADefaultPlayerState * ps = GetPlayerState<ADefaultPlayerState>();
	if (IsValid(ps) && ps->IsClientInitialized() && bisregistered == true)
	{
		PostRegisterInit();
	}
}

bool ADefaultPlayerController::ReadyForUIInit() const
{
	bool retval = IsRegistered() && IsLocalPlayerController();

	if (retval)
	{
		const UWorld* world = GetWorld();
		const ADefaultGameState* gs = world->GetGameState<ADefaultGameState>();
		const ADefaultPlayerState* ps = GetPlayerState<ADefaultPlayerState>();

		if (ps && gs)
		{
			retval = ps->IsClientInitialized() && gs->IsClientDataReady();
		}
	}


	return retval;
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

void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("EscapeKey", IE_Pressed, this, &ADefaultPlayerController::OnEscapeActionPressed);
}

void ADefaultPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	if (IsLocalPlayerController() && (GetNetMode() != NM_DedicatedServer)  && IsValid(PlayerState))
	{
		GetPlayerState<ADefaultPlayerState>()->SetLocalPlayerState(true);
	}
}

void ADefaultPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
	ULobbyGameInstance* gi = GetGameInstance<ULobbyGameInstance>();
	gi->StartLoadingLevel(FURL(*PendingURL), TravelType, bIsSeamlessTravel);
}

void ADefaultPlayerController::PostRegisterInit()
{
	/*Client Will inititialize its UI After Gamestate's initial Replication, Server can Start Here*/
	if (IsLocalPlayerController() && HasAuthority())
	{
		ClientInitUI();
	}
	else if (battemptedHudinit == true)
	{
		/*If Player Registration was Behind Gamestate Replication*/
		ADefaultHUD * hud = GetHUD<ADefaultHUD>();
		isHUDInitialized = hud->ClientInitializeHUD();
	}

	FinishLocalPlayerSetup();
}

void ADefaultPlayerController::ClientInitUI()
{
	if (ReadyForUIInit())
	{
		ADefaultHUD * hud = GetHUD<ADefaultHUD>();
		isHUDInitialized = hud->ClientInitializeHUD();
	}
}

bool ADefaultPlayerController::GetPlayerInfo(FPlayerSettings& outsettings) const
{
	const ULobbyGameInstance * GI = GetGameInstance<ULobbyGameInstance>();
	if (GI == nullptr) return false;

	outsettings = GI->GetPlayerSettings();

	return (outsettings.bIsValid);
}

void ADefaultPlayerController::OpenExternalMenu(UUserWidget* InMenu)
{
	if (InMenu == nullptr || !InMenu->IsValidLowLevel()) return;

	InMenu->AddToViewport();
	bShowMouseCursor = true;
	DisableInput(this);
	FInputModeUIOnly inputMode;
	SetInputMode(inputMode);
	EnableInput(this);

	ExternalMenu = InMenu;
}

bool ADefaultPlayerController::IsExternalMenuOpen() const
{
	return ExternalMenu != nullptr;
}

void ADefaultPlayerController::CloseExternalMenu()
{
	if (ExternalMenu == nullptr) return;

	ExternalMenu->RemoveFromParent();
	FInputModeGameOnly inputMode;
	inputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(inputMode);
	bShowMouseCursor = false;
	ExternalMenu = nullptr;
}

void ADefaultPlayerController::OnBeginPause()
{
}

void ADefaultPlayerController::OnEndPause()
{
}

void ADefaultPlayerController::OnEscapeActionPressed()
{
	ADefaultHUD* defaulthud = GetHUD<ADefaultHUD>();
	defaulthud->OnEscapeAction();
}
