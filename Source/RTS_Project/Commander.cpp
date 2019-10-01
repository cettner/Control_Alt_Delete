// Fill out your copyright notice in the Description page of Project Settings.
#include "Commander.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Engine.h"
#include "FPSServerController.h"
#include "UnrealNetwork.h"
#include "DefaultPlayerState.h"

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

void ACommander::Tick(float DeltaTime)
{
	FHitResult hit;
	FVector fwd = FPS_Camera->GetForwardVector();
	FVector start = FPS_Camera->GetComponentLocation();
	FVector end = (fwd * 500.0f) + start;
	

	//DrawDebugLine(GetWorld(), start, end, FColor(255, 0, 0), false, -1, 0, 12.33);
	AActor * hitSelectable = GetSelectableActor();
	if (hitSelectable)
	{
		//if (Cast<ARTSMinion>(hit.GetActor()))
		//{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Hit!")));
		//}
	}
}

ACommander::ACommander()
{
	FPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FPS_Camera->SetupAttachment(GetCapsuleComponent());
	FPS_Camera->RelativeLocation = FVector(-39.56f, 1.75f, 64.f);
	FPS_Camera->bUsePawnControlRotation = true;

	FPS_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPS_Mesh"));
	FPS_Mesh->SetOnlyOwnerSee(true);
	FPS_Mesh->SetupAttachment(FPS_Camera);
	FPS_Mesh->bCastDynamicShadow = false;
	FPS_Mesh->CastShadow = false;
	FPS_Mesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FPS_Mesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	trace = FCollisionQueryParams(FName(TEXT("FPSTrace")), true, this);
	trace.bTraceComplex = true;
	trace.bReturnPhysicalMaterial = false;
	trace.AddIgnoredActor(this);

	PrimaryActorTick.bCanEverTick = Role != ROLE_Authority;
	PrimaryActorTick.bStartWithTickEnabled = Role != ROLE_Authority;
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

bool ACommander::IsFirstPerson()
{
	return IsAlive() && Controller && Controller->IsLocalPlayerController();
}

void ACommander::SetTarget(AActor * newtarget)
{

}

bool ACommander::CanInteract(AActor * Interactable)
{
	bool retval = false;
	if (Cast<ARTSMinion>(Interactable))
	{
		retval = true;
	}
	else if (Cast<ARTSSelectable>(Interactable))
	{
		retval = true;
	}

	return(retval);
}

bool ACommander::GetMarchingOrder(ARTSMinion * needs_orders, FVector &OutVector)
{
	int index = Squad.IndexOfByKey(needs_orders);
	if (index >= 0)
	{
		if (form == SQUARE)
		{
			OutVector = GetSquareFormation(index,marchwidth);
			return(true);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Formation Requested!")));
		}
	}
	return(false);
}

ACommander * ACommander::GetCommander()
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
	if (Squad.Contains(squadmate)  || squadmate->GetCommander())
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

bool ACommander::SelectableInterationHandler_Validate(ARTSSelectable * Interacted)
{
	if (Interacted)
	{
		return(true);
	}
	return(false);
}

void ACommander::Interact()
{
	AActor * hittarget = GetSelectableActor();

	if(CanInteract(hittarget))
	{
		AFPSServerController * Server = Cast<AFPSServerController>(GetController());
		if (Server && Server->Server_Request_Interact_Validate(this,hittarget))
		{
			Server->Server_Request_Interact_Implementation(this, hittarget);
		}
	} 
}

bool ACommander::MinionInteractionHandler_Validate(ARTSMinion * Interacted)
{
	if (Interacted)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

void ACommander::MinionInteractionHandler_Implementation(ARTSMinion * Interacted)
{
		if (!AddtoSquad(Interacted))
		{
			LeaveSquad(Interacted);
		}
}

void ACommander::SelectableInterationHandler_Implementation(ARTSSelectable * Interacted)
{
	for(int i = 0; i < Squad.Num();  i++)
	{
		Squad[i]->SetTarget(Interacted);
	}
}

FVector ACommander::GetSquareFormation(int index, float width)
{
	FVector mylocation = GetActorLocation();
	int corner = index % 4;


	float xcalc;
	float ycalc;

	if (corner == 0)
	{
		xcalc = width + (width)*(index / 4);
		ycalc = width + (width)*(index / 4);
	}
	else if(corner == 1)
	{
		xcalc = width + (width)*(index / 4);
		ycalc = -width - (width)*(index / 4);
	}
	else if (corner == 2)
	{
		xcalc = -width - (width)*(index / 4);
		ycalc = width + (width)*(index / 4);
	}
	else
	{
		xcalc = -width - (width)*(index / 4);
		ycalc = -width - (width)*(index / 4);
	}

	FVector RetVal = mylocation + FVector(xcalc, ycalc, 0);

	return (RetVal);
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
	/*E*/
	ActorInputComponent->BindAction("Interact", IE_Pressed, this, &ACommander::Interact);
	
}

void ACommander::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACommander, Squad);
}