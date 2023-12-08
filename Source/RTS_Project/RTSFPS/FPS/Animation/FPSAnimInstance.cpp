// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAnimInstance.h"
#include "../CombatCommander.h"
#include "KismetAnimationLibrary.h"

#include "GameFramework/PawnMovementComponent.h"

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	 
	APawn * Pawn =  TryGetPawnOwner();

	if (Pawn)
	{
		const FVector velocity = Pawn->GetVelocity();
		
		Speed = velocity.Size();
		Direction = UKismetAnimationLibrary::CalculateDirection(velocity, Pawn->GetActorRotation());
		bIsMoving = Speed > 0.0F;

		UpdateAimOffset(Pawn);

		const UPawnMovementComponent * MoveComp = Pawn->GetMovementComponent();

		if (MoveComp)
		{
			bIsFalling = MoveComp->IsFalling();
		}
	}

	const ACombatCommander * WeaponHolder = Cast<ACombatCommander>(Pawn);

	if (WeaponHolder)
	{
		WeaponStance = WeaponHolder->GetWeaponStance();
	}
}

void UFPSAnimInstance::UpdateAimOffset(APawn * AnimPawn)
{
	if (AnimPawn->HasAuthority() || AnimPawn->IsLocallyControlled())
	{
		const FRotator controllerrotation = AnimPawn->GetControlRotation();
		Pitch = FMath::ClampAngle(controllerrotation.Pitch, -90.0f, 90.0f);
	}
	else
	{
		const float remotepitch = FRotator::DecompressAxisFromByte(AnimPawn->RemoteViewPitch);
		Pitch = FMath::ClampAngle(remotepitch, -90.0f, 90.0f);;
	}
}
