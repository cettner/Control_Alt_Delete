// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RTSMinion.h"
#include "UObject/ConstructorHelpers.h"
#include "RTSSelectionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "RTSAIController.h"
#include"Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include"Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "GameAssets.h"
#include "Commander.h"
#include "UnrealNetwork.h"
#include "DefaultPlayerState.h"

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
	return (Health->IsAlive());
}

bool ARTSMinion::IsEnemy(AActor* FriendOrFoe)
{
	bool Enemy = false;
	ARTSMinion * InMinion = Cast<ARTSMinion>(FriendOrFoe);
	ARTSStructure* InStructure = Cast<ARTSStructure>(FriendOrFoe);

	if (InMinion && InMinion->team_index != this->team_index && InMinion->team_index >= 0)
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
	Selection->SetSelected();
}

void ARTSMinion::SetDeselected()
{
	Selection->SetDeselected();
}

void ARTSMinion::SetUnselectable()
{
	Selection->EnableSecondary();
}

void ARTSMinion::SetSelectable()
{
	Selection->DisableSecondary();
}

ACommander * ARTSMinion::GetCommander()
{
	if (Role == ROLE_Authority)
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

int ARTSMinion::GetTeam()
{
	return team_index;
}

UBehaviorTree* ARTSMinion::GetBehavior()
{
	return(RTSBehavior);
}

//interface function for override;
void ARTSMinion::ReleaseAssets()
{
	ClearTarget();
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

void ARTSMinion::BeginPlay()
{
	Super::BeginPlay();
	if (Role != ROLE_Authority && GetWorld())
	{
		/*Set Enemy Selection Rings For Local Player*/
		APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC && Cast<ADefaultPlayerState>(PC->PlayerState))
		{
			if (GetTeam() != Cast<ADefaultPlayerState>(PC->PlayerState)->Team_ID)
			{
				SetUnselectable();
			}
			else
			{
				SetSelectable();
			}
		}
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