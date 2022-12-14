// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAnimInstance.h"


void UMinionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
