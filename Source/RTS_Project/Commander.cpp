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
	
	FCollisionQueryParams trace = FCollisionQueryParams(FName(TEXT("FPSTrace")), true, this);
	trace.bTraceComplex = true;
	trace.bTraceAsyncScene = true;
	trace.bReturnPhysicalMaterial = false;
	trace.AddIgnoredActor(this);

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

void ACommander::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveForward", this, &ACommander::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACommander::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACommander::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &ACommander::AddControllerPitchInput);
}