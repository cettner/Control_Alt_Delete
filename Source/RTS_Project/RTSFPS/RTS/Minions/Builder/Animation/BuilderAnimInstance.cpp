// Fill out your copyright notice in the Description page of Project Settings.


#include "BuilderAnimInstance.h"
#include "..\RTSBUILDER.h"

void UBuilderAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const ARTSBUILDER* builder = Cast<ARTSBUILDER>(TryGetPawnOwner());

	if (IsValid(builder))
	{
		bIsMining = builder->IsMining();
	}
}
