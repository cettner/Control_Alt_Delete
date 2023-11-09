// Fill out your copyright notice in the Description page of Project Settings.
#include "Commander.h"
#include "FPSPlayerstate.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/MenuInteractableInterface.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"

#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

float ACommander::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	USkeletalMeshComponent* UseMesh = GetPawnMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance)
	{
		return UseMesh->AnimScriptInstance->Montage_Play(AnimMontage, InPlayRate);
	}

	return 0.0f;
}

void ACommander::StopAnimMontage(UAnimMontage* AnimMontage)
{
	USkeletalMeshComponent* UseMesh = GetPawnMesh();
	if (AnimMontage && UseMesh && UseMesh->AnimScriptInstance &&
		UseMesh->AnimScriptInstance->Montage_IsPlaying(AnimMontage))
	{
		UseMesh->AnimScriptInstance->Montage_Stop(AnimMontage->BlendOut.GetBlendTime(), AnimMontage);
	}
}

void ACommander::OnPlayerStateChanged(APlayerState* NewPlayerState, APlayerState* OldPlayerState)
{
	Super::OnPlayerStateChanged(NewPlayerState, OldPlayerState);

	AFPSPlayerState * PS = Cast<AFPSPlayerState>(NewPlayerState);
	if (PS == nullptr) return;
	SetTeam(PS->GetTeamID());

	/*Load Our UpgradeSet and register for new ones*/
	UUpgradeData* upgradeset = PS->GetUpgradeData();
	if (upgradeset != nullptr)
	{
		const TArray<TSubclassOf<UUpgrade>> knownupgrades = upgradeset->GetKnownUpgrades();
		for (TSubclassOf<UUpgrade> upgrade : knownupgrades)
		{
			const uint32 currentrank = upgradeset->GetCurrentUpgradeRankFor(upgrade);
			OnUpgradeChanged(upgrade, UPGRADE_UNLEARNED, currentrank);
		}

		upgradeset->OnUpgradeChanged.AddUFunction(this, "OnUpgradeChanged");
	}
}

ACommander::ACommander() : Super()
{
	FPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FPS_Camera->SetupAttachment(RootComponent);
	FPS_Camera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FPS_Camera->bUsePawnControlRotation = true;

	FPS_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS_Mesh"));
	FPS_Mesh->SetOnlyOwnerSee(true);
	FPS_Mesh->SetupAttachment(FPS_Camera);
	FPS_Mesh->bCastDynamicShadow = false;
	FPS_Mesh->CastShadow = false;
	FPS_Mesh->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	FPS_Mesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	FPS_Mesh->bReceivesDecals = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	trace = FCollisionQueryParams(FName(TEXT("FPSTrace")), true, this);
	trace.bTraceComplex = true;
	trace.bReturnPhysicalMaterial = false;
	trace.AddIgnoredActor(this);

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ACommander::MoveForward(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();

		UCharacterMovementComponent * Movecomp = GetCharacterMovement();
		
		// Limit pitch when walking or falling
		if (Movecomp->IsMovingOnGround() || Movecomp->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void ACommander::MoveRight(float Val)
{
	if ((Controller != NULL) && (Val != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Val);
	}
}

AActor * ACommander::GetSelectableActor()
{
	FHitResult hit;
	FVector fwd = FPS_Camera->GetForwardVector();
	FVector start = FPS_Camera->GetComponentLocation();
	FVector end = (fwd * 500.0f) + start;

	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, SELECTION_CHANNEL, trace))
	{
			return(hit.GetActor());
	}
	else
	{
		return(nullptr);
	}
}

bool ACommander::IsFirstPerson() const
{
	bool retval = false;
	const bool balive = IsAlive();
	if (!HasAuthority())
	{
		const bool blocalControl = ((Controller != nullptr) && Controller->IsLocalPlayerController());
		retval = balive && blocalControl;
	}
	else
	{
		const bool blocalControl = ((Controller != nullptr) && Controller->IsLocalPlayerController());
		const bool bIsServerInitPawn = IsServerPawn();
		retval = balive && (bIsServerInitPawn || blocalControl);
	}

	return retval;
}

int ACommander::GetTeam() const
{
	ADefaultPlayerState* ps = GetPlayerState<ADefaultPlayerState>();
	if (ps == nullptr) return TeamID;

	const int retval = ps->GetTeamID();
	return retval;
}

void ACommander::SetTeam(int InTeamID)
{
	/*We dont need Authoritative control on the TeamID because we get it from the playerstate*/
	TeamID = InTeamID;
	if (!(GetNetMode() == NM_DedicatedServer && HasAuthority()))
	{
		OnRep_TeamID();
	}
}

bool ACommander::IsServerPawn() const
{
	return bIsServerPawn;
}

void ACommander::SetIsServerPawn(bool IsServer)
{
	bIsServerPawn = IsServer;
}

void ACommander::OnDeath()
{
	Super::OnDeath();
}

void ACommander::IssueOrder(AController* Issuer, const FHitResult& InHitContext, URTSOrder* InOrder, const bool InbIsQueuedOrder)
{
	/*TODO:: Add waypoint highlighting for incoming order*/
}

IRTSObjectInterface * ACommander::GetLeadRTSObject()
{
	return(this);
}

USkeletalMeshComponent * ACommander::GetSpecifcPawnMesh(bool WantFirstPerson) const
{
	return WantFirstPerson == true ? FPS_Mesh : GetMesh();
}

USkeletalMeshComponent* ACommander::GetPawnMesh()
{
	return IsFirstPerson() ? FPS_Mesh : GetMesh();
}

void ACommander::ClearCommander()
{
	//Do nothing, We are a Commander
}

void ACommander::SetCommander(ACommander * Commander)
{
	//Currently do nothing as Commanders cannot squad other commanders
}

bool ACommander::AddtoSquad(ARTSMinion * squadmate)
{
	if (Squad.Contains(squadmate)  || squadmate->GetLeadRTSObject())
	{
		return(false);
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Minion Added to Squad!")));
	Squad.Add(squadmate);
	squadmate->SetCommander(this);
	return(true);
}

bool ACommander::LeaveSquad(ARTSMinion * leaver)
{
	if (Squad.Contains(leaver))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Minion Removed From Squad!")));
		Squad.Remove(leaver);
		leaver->ClearCommander();
		return(true);
	}
	else
	{
		return(false);
	}
}

bool ACommander::SelectableInterationHandler_Validate(AActor * Interacted)
{
	return(true);
}

void ACommander::Interact()
{
	AActor * hittarget = GetSelectableActor();
}

bool ACommander::MinionInteractionHandler_Validate(ARTSMinion * Interacted)
{
	return(true);
}

void ACommander::MinionInteractionHandler_Implementation(ARTSMinion * Interacted)
{
}

void ACommander::SelectableInterationHandler_Implementation(AActor * Interacted)
{
}

void ACommander::SetupPlayerInputComponent(UInputComponent* ActorInputComponent)
{
	// set up gameplay key bindings
	/*W*/
	ActorInputComponent->BindAxis("MoveForward", this, &ACommander::MoveForward);
	/*D*/
	ActorInputComponent->BindAxis("MoveRight", this, &ACommander::MoveRight);
	/*Mouse X*/
	ActorInputComponent->BindAxis("Turn", this, &ACommander::AddControllerYawInput);
	/*Mouse Y*/
	ActorInputComponent->BindAxis("LookUp", this, &ACommander::AddControllerPitchInput);
	/*F*/
	ActorInputComponent->BindAction("FKey", IE_Pressed, this, &ACommander::Interact);
	
}

void ACommander::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACommander, Squad);
	/*We Get This From the PLayerState so we dont need to replicate the teamid for the pawn*/
	DISABLE_REPLICATED_PROPERTY(ACommander, TeamID);
}