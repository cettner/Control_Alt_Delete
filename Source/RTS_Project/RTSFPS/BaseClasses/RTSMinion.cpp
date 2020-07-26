// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/AssetHelpers/GameAssets.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/GameArchitecture/Game/DefaultPlayerState.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSSelectionComponent.h"

#include "UObject/ConstructorHelpers.h"
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

	//AI recives Access on Spawn
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// No Ticking on Minions
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	Selection = CreateDefaultSubobject<URTSSelectionComponent>(TEXT("Selection"));
	Selection->SetRoot(RootComponent);
	Selection->SetDetection(GetCapsuleComponent());

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Health->OnDeathStart.BindUFunction(this,"OnDeath");
	Health->SetIsReplicated(true);

	AIConfig.LoseSightRadius = 2000.0;
	/*Radial from forward vector ie: 180 == 180 * 2*/
	AIConfig.PeripheralVision = 180.0;
	AIConfig.SightRadius = 1500;

	FAISenseAffiliationFilter Affiliations;
	Affiliations.bDetectEnemies = true;
	Affiliations.bDetectFriendlies = false;
	Affiliations.bDetectNeutrals = false;

	AIConfig.SightAffiliation = Affiliations;
}

void ARTSMinion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UWorld* World = GetWorld();
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
	PerceptionSystem->RegisterSourceForSenseClass(UAISense_Sight::StaticClass(), *this);
}

bool ARTSMinion::CanInteract(AActor * Interactable)
{
	//By Default Can Only Interact with Minions and Structures
	return(Cast<ARTSMinion>(Interactable) || Cast<ARTSStructure>(Interactable));
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

bool ARTSMinion::IsAlive()
{
	return (Health && Health->IsAlive());
}

void ARTSMinion::OnDeath()
{
	UWorld* World = GetWorld();
	UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(World);
	PerceptionSystem->UnregisterSource(*this);
}

bool ARTSMinion::IsEnemy(AActor* FriendOrFoe)
{
	bool Enemy = false;
	ARTSMinion * InMinion = Cast<ARTSMinion>(FriendOrFoe);
	ARTSStructure* InStructure = Cast<ARTSStructure>(FriendOrFoe);

	if (InMinion && InMinion->GetTeam() != this->GetTeam() && InMinion->GetTeam() >= 0)
	{
		Enemy = true;
	}
	else if(InStructure && InStructure->teamindex != this->GetTeam() && InStructure->teamindex >= 0)
	{
		Enemy = true;
	}

	return(Enemy);
}

AActor * ARTSMinion::GetTarget()
{
	ARTSAIController * rtscontrol = Cast<ARTSAIController>(GetController());
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

	ARTSAIController * rtscontrol = Cast<ARTSAIController>(GetController());
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
		Selection->SetSelected();
	}
}

void ARTSMinion::SetDeselected()
{
	if (Selection)
	{
		Selection->SetDeselected();
	}
}

void ARTSMinion::SetTeamColors()
{
	if (Selection)
	{
		Selection->SetSelectionColor(FLinearColor::Red);
	}
}

ACommander * ARTSMinion::GetCommander()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
		ACommander * commander = AIC->GetCommander();

		//Do a double check to make sure replicated commander and blackboard are in sync, should never happen
		if (Cmdr != commander)
		{
			Cmdr = commander;
		}

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
	ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
	AIC->SetCommander(Commander);
	Cmdr = Commander;
}

void ARTSMinion::SetTeam(int team_id)
{
	team_index = team_id;
}

int ARTSMinion::GetTeam() const
{
	return team_index;
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

}

bool ARTSMinion::HasAssets()
{
	if(GetTarget())
	{
		return(true);
	}
	return (false);
}

void ARTSMinion::RtsMove(FVector Local)
{
	ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
	if (AIC != NULL)
	{
		AIC->MoveToLocation(Local, 5.0f, false, true, true, true, 0, false);
	}
}

void ARTSMinion::RtsMoveToActor(AActor * move_to_me)
{
	ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
	if (AIC != NULL)
	{
		AIC->MoveToActor(move_to_me, 5.0f, true, true, false, 0, false);
	}
}

float ARTSMinion::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
 	float ProcessedDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	ProcessedDamage = Health->HandleDamageEvent(ProcessedDamage, DamageEvent, EventInstigator, DamageCauser);

	return (ProcessedDamage);
}

UTexture * ARTSMinion::GetThumbnail()
{
	return Thumbnail;
}

void ARTSMinion::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARTSMinion, Cmdr);
	DOREPLIFETIME(ARTSMinion, team_index)
}