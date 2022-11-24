// Fill out your copyright notice in the Description page of Project Settings.


#include "ScionAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

void UScionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn * Pawn =  TryGetPawnOwner();

	if (Pawn)
	{
		const FVector velocity = Pawn->GetVelocity();
		
		Speed = velocity.Size();
		Direction = CalculateDirection(velocity, Pawn->GetActorRotation());
	}

}
