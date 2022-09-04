// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultPlayerState.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"



ARTSMinion::ARTSMinion()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	/*SelectionRings Shouldnt appear on mesh*/
	GetMesh()->bReceivesDecals = false;
	/*Execute Montages even when not visible by the server, as socket locations are always relevant*/
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	//AI recives Access on Spawn
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// No Ticking on Minions
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Selection = CreateDefaultSubobject<UDecalSelectionComponent>(TEXT("Selection"));
	Selection->SetDetection(GetCapsuleComponent());
	Selection->SetupAttachment(RootComponent);
	SetDeselected();

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->OnDeathStart.BindUFunction(this,"OnDeath");
	Health->SetIsReplicated(true);

	AIConfig.LoseSightRadius = 2000.0;
	/*Radial from forward vector ie: 180 == abs(0 +- 90)*/
	AIConfig.PeripheralVision = 180.0;
	AIConfig.SightRadius = 1500;

	FAISenseAffiliationFilter Affiliations = FAISenseAffiliationFilter();
	Affiliations.bDetectEnemies = true;
	Affiliations.bDetectFriendlies = false;
	Affiliations.bDetectNeutrals = false;

	AIConfig.SightAffiliation = Affiliations;
}

void ARTSMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
		if (PerceptionSystem)
		{
			PerceptionSystem->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
		}
	}
}

void ARTSMinion::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		bAreComponentsReadyforUpgrades = true;
		PostInstallUpgrades();
	}
	
	RegisterRTSObject();
}

bool ARTSMinion::CanInteract(AActor * Interactable)
{
	bool retval = Cast<IRTSObjectInterface>(Interactable) != nullptr;
	return(retval);
}

bool ARTSMinion::CanAttack(AActor * AttackMe)
{
	return (false);
}

bool ARTSMinion::CanDoDamage(AActor * AttackMe)
{
	return (false);
}

void ARTSMinion::StartAttack(AActor * AttackMe)
{
}

bool ARTSMinion::IsAlive() const
{
	const bool retval = Health && Health->IsAlive();
	return (retval);
}

bool ARTSMinion::IsBoxSelectable() const
{
	return bIsBoxSelectable;
}

void ARTSMinion::RegisterRTSObject()
{
	const UWorld * world = GetWorld();
	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();

	/*This can miss during game startup for actors placed on the map it's corrected in ARTSFPSGamestate::RefreshAllUnits()*/
	if (IsValid(gs))
	{
		gs->RegisterRTSObject(this);
	}
}

void ARTSMinion::UnRegisterRTSObject()
{
	const UWorld* world = GetWorld();
	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	gs->UnRegisterRTSObject(this);
}

void ARTSMinion::OnDeath()
{
	if (HasAuthority())
	{
		/*Unregister Minion to be percieved from AI perception*/
		UWorld* World = GetWorld();
		UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
		PerceptionSystem->UnregisterSource(*this);

		/*Notify the Gamestate We Died*/
		ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();
		GS->OnUnitDeath(this);
	}

	ARTSHUD * hud = GetWorld()->GetFirstPlayerController()->GetHUD<ARTSHUD>();
	if (hud)
	{
		hud->ForceRemoveSelection(this);
	}

	UnRegisterRTSObject();

}

bool ARTSMinion::IsEnemy(AActor* FriendOrFoe)
{
	bool Enemy = false;
	if (!IsValid(FriendOrFoe)) return false;

	IRTSObjectInterface* rtsobj = Cast<IRTSObjectInterface>(FriendOrFoe);

	if (rtsobj && rtsobj->GetTeam() != this->GetTeam() && rtsobj->GetTeam() >= 0)
	{
		Enemy = true;
	}

	return(Enemy);
}

AActor * ARTSMinion::GetTarget()
{
	ARTSAIController * rtscontrol = GetController<ARTSAIController>();
	if (rtscontrol)
	{
		return(rtscontrol->GetTarget());
	}
	else
	{
		return(nullptr);
	}
}

void ARTSMinion::SetTarget(AActor * NewTarget)
{
	ClearTarget();

	ARTSAIController * rtscontrol = GetController<ARTSAIController>();
	if (rtscontrol)
	{
		rtscontrol->SetTarget(NewTarget);
	}
}

void ARTSMinion::ClearTarget()
{
	ARTSAIController * rtscontrol = Cast<ARTSAIController>(GetController());
	if (rtscontrol)
	{
		rtscontrol->ClearTarget();
	}
}

void ARTSMinion::SetSelected()
{
	if (Selection)
	{
		Selection->SetHiddenInGame(false);
	}
}

void ARTSMinion::SetDeselected()
{
	if (Selection)
	{
		Selection->SetHiddenInGame(true);
	}
}

void ARTSMinion::SetTeamColors(FLinearColor TeamColor)
{
	if (Selection)
	{
		Selection->SetSelectionColor(TeamColor);
	}
}

IRTSObjectInterface * ARTSMinion::GetLeadRTSObject()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ARTSAIController * AIC = GetController<ARTSAIController>();
		ACommander * commander = AIC->GetLeadRTSObject();

		return(commander);
	}
	else
	{
		return(Cmdr);
	}

}

void ARTSMinion::ClearCommander()
{
	ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
	AIC->ClearCommander();
	Cmdr = nullptr;
}

void ARTSMinion::SetCommander(ACommander * Commander)
{
	ARTSAIController * AIC = GetController<ARTSAIController>();
	AIC->SetCommander(Commander);
	Cmdr = Commander;
}

void ARTSMinion::SetTeam(int team_id)
{
	TeamID = team_id;

	if (HasAuthority() && (GetNetMode() == NM_DedicatedServer))
	{
		OnRep_TeamID();
	}
}

int ARTSMinion::GetTeam() const
{
	return TeamID;
}

void ARTSMinion::OnLocalPlayerTeamChange(int InLocalTeamID)
{
	OnRep_TeamID();
}

void ARTSMinion::OnRep_TeamID()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	
	ADefaultPlayerController * PC = World->GetFirstPlayerController<ADefaultPlayerController>();
	if (PC == nullptr) return;

	ADefaultPlayerState* PS = PC->GetPlayerState<ADefaultPlayerState>();
	if (PS && PS->GetTeamID() != TeamID)
	{
		SetTeamColors(FLinearColor::Red);
		SetSelected();
	}
	else
	{
		/*Enable box selection*/
		bIsBoxSelectable = true;
		SetDeselected();
	}
}

UBehaviorTree* ARTSMinion::GetBehavior()
{
	return(RTSBehavior);
}

FRTSAIPerceptionConfig ARTSMinion::GetAIConfig() const
{
	return AIConfig;
}

//interface function for override;
void ARTSMinion::ReleaseAssets()
{
	ARTSAIController* AIC = GetController<ARTSAIController>();
	AIC->ReleaseAssets();
}

bool ARTSMinion::HasAssets()
{
	if(GetTarget())
	{
		return(true);
	}
	return (false);
}

void ARTSMinion::PostInstallUpgrades()
{
	for (int i = 0; i < AppliedUpgrades.Num(); i++)
	{
		const UUpgrade * upgrade = AppliedUpgrades[i].GetDefaultObject();
		upgrade->ApplyUpgrade(this);
	}
}

bool ARTSMinion::CanReceiveUpgrades() const
{
	return bAreComponentsReadyforUpgrades;
}

bool ARTSMinion::AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd)
{
	AppliedUpgrades.Emplace(UpgradeToAdd);
	return true;
}

float ARTSMinion::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
 	float ProcessedDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	ProcessedDamage = Health->HandleDamageEvent(ProcessedDamage, DamageEvent, EventInstigator, DamageCauser);

	return (ProcessedDamage);
}

void ARTSMinion::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARTSMinion, Cmdr);
	DOREPLIFETIME(ARTSMinion, TeamID)
}