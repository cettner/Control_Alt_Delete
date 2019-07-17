// Fill out your copyright notice in the Description page of Project Settings.
#include "Commander.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Engine.h"
#include "FPSServerController.h"
#include "UnrealNetwork.h"

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
	FPS_Camera->SetupAttachment(GetMesh(), TEXT("FPPCamera_socket"));
	FPS_Camera->bUsePawnControlRotation = true;

	trace = FCollisionQueryParams(FName(TEXT("FPSTrace")), true, this);
	trace.bTraceComplex = true;
	trace.bTraceAsyncScene = true;
	trace.bReturnPhysicalMaterial = false;
	trace.AddIgnoredActor(this);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
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

void ACommander::PrimaryPressed()
{
	Swing_Weapon = true;
}

void ACommander::PrimaryReleased()
{
	Swing_Weapon = false;
}

void ACommander::SecondaryPressed()
{
	blocking = true;
}

void ACommander::SecondaryReleased()
{
	blocking = false;
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

void ACommander::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	/*W*/
	InputComponent->BindAxis("MoveForward", this, &ACommander::MoveForward);
	/*D*/
	InputComponent->BindAxis("MoveRight", this, &ACommander::MoveRight);
	/*Mouse X*/
	InputComponent->BindAxis("Turn", this, &ACommander::AddControllerYawInput);
	/*Mouse Y*/
	InputComponent->BindAxis("LookUp", this, &ACommander::AddControllerPitchInput);
	/*E*/
	InputComponent->BindAction("Interact", IE_Pressed, this, &ACommander::Interact);

	InputComponent->BindAction("RightMouse", IE_Pressed, this, &ACommander::SecondaryPressed);

	InputComponent->BindAction("RightMouse", IE_Released, this, &ACommander::SecondaryReleased);  

	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ACommander::PrimaryPressed);

	InputComponent->BindAction("LeftMouse", IE_Released, this, &ACommander::PrimaryReleased);
	
}


void ACommander::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACommander, Squad);
}