// Fill out your copyright notice in the Description page of Project Settings.
#include "Commander.h"
#include "Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"



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