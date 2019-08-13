// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPlayerState.h"
#include "DefaultGameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

void ADefaultPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ADefaultPlayerState::BeginPlay()
{
	if (HasAuthority())
	{
		UWorld * World = GetWorld();
		if (World && World->GetGameState<ADefaultGameState>())
		{
			Team_ID = World->GetGameState<ADefaultGameState>()->AssignBalancedTeam(this);
		}
	}
}

void ADefaultPlayerState::ClientInitialize(AController * C)
{
	Super::ClientInitialize(C);
}

ADefaultPlayerState::ADefaultPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ADefaultPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADefaultPlayerState, Team_ID);
}