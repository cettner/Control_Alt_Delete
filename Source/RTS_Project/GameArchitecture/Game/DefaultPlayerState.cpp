// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPlayerState.h"
#include "DefaultPlayerController.h"
#include "DefaultGameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

void ADefaultPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADefaultPlayerState::ClientInitialize(AController* Controller)
{
	Super::ClientInitialize(Controller);

	ADefaultPlayerController* PC = Cast<ADefaultPlayerController>(Controller);
	if (PC)
	{
		PC->ClientRequestRegistration();
	}
}

ADefaultPlayerState::ADefaultPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamID = -1;
}

void ADefaultPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADefaultPlayerState, TeamID);
}