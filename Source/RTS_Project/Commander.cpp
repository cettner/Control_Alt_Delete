// Fill out your copyright notice in the Description page of Project Settings.
#include "Commander.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Engine.h"


void ACommander::Tick(float DeltaTime)
{
	FHitResult hit;
	FVector fwd = FPS_Camera->GetForwardVector();
	FVector start = FPS_Camera->GetComponentLocation();
	FVector end = (fwd * 250.0f) + start;
	

	DrawDebugLine(GetWorld(), start, end, FColor(255, 0, 0), false, -1, 0, 12.33);

	if (GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, trace))
	{
		if (Cast<ARTSMinion>(hit.GetActor()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Hit!")));
		}
	}
}

ACommander::ACommander()
{
	FPS_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FPS_Camera->SetupAttachment(GetMesh(), TEXT("Head"));

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

FVector ACommander::GetMarchingOrder(ARTSMinion * needs_orders)
{
	int index = Squad.IndexOfByKey(needs_orders);
	if (index >= 0)
	{
		if (form == SQUARE)
		{
			return(GetSquareFormation(index,marchwidth));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Formation Requested!")));
		}
	}
	return(FVector());
}

ACommander * ACommander::GetCommander()
{
	return(this);
}

bool ACommander::AddtoSquad(ARTSMinion * squadmate)
{
	if (Squad.Contains(squadmate))
	{
		return(false);
	}
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Minion Added to Squad!")));
	Squad.Add(squadmate);
	return(true);
}

bool ACommander::LeaveSquad(ARTSMinion * leaver)
{
	if (Squad.Contains(leaver))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Minion Removed From Squad!")));
		Squad.Remove(leaver);
		return(true);
	}
	else
	{
		return(false);
	}
	
}

void ACommander::Interact()
{

}

void ACommander::PrimaryFire()
{
}

void ACommander::SecondaryFire()
{
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
	InputComponent->BindAxis("MoveForward", this, &ACommander::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACommander::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACommander::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &ACommander::AddControllerPitchInput);
	
	InputComponent->BindAction("Interact", IE_Pressed, this, &ACommander::Interact);
	
}