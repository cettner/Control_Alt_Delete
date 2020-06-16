// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPlayerState.h"
#include "DefaultGameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

void ADefaultPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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