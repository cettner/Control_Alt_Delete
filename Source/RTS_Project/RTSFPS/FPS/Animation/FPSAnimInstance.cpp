// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAnimInstance.h"
#include "../CombatCommander.h"

#include "GameFramework/PawnMovementComponent.h"

void UFPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn * Pawn =  TryGetPawnOwner();

	if (Pawn)
	{
		FVector velocity = Pawn->GetVelocity();
		
		Speed = velocity.Size();
		Direction = CalculateDirection(velocity, Pawn->GetActorRotation());
		bIsMoving = Speed > 0.0F;

		UPawnMovementComponent * MoveComp = Pawn->GetMovementComponent();

		if (MoveComp)
		{
			bIsFalling = MoveComp->IsFalling();
		}
	}

	ACombatCommander * WeaponHolder = Cast<ACombatCommander>(Pawn);

	if (WeaponHolder)
	{
		WeaponStance = WeaponHolder->GetWeaponStance();
	}
}
