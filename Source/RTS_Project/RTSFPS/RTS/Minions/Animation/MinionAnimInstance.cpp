// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionAnimInstance.h"
#include "KismetAnimationLibrary.h"

void UMinionAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn * Pawn =  TryGetPawnOwner();

	if (Pawn)
	{
		const FVector velocity = Pawn->GetVelocity();
		
		Speed = velocity.Size();
		Direction = UKismetAnimationLibrary::CalculateDirection(velocity, Pawn->GetActorRotation());// CalculateDirection();
	}

}
