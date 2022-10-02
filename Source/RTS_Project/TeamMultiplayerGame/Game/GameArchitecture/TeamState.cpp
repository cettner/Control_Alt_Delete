// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamState.h"
#include "DefaultPlayerController.h"
#include "DefaultGameState.h"

#include "Net/UnrealNetwork.h"


ATeamState::ATeamState() : Super()
{
	bReplicates = true;
}

bool ATeamState::AddPlayer(ADefaultPlayerState* InPlayerstate)
{
	return false;
}

bool ATeamState::RemovePlayer(ADefaultPlayerState* InPlayerstate)
{
	return false;
}

void ATeamState::OnRep_TeamID()
{
}

bool ATeamState::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool retval = false;
	const ADefaultPlayerController* pc = Cast<ADefaultPlayerController>(RealViewer);
	
	if (pc != nullptr)
	{
		retval = (TeamID == pc->GetTeamID()) && (TeamID != -1);
	}
	return retval;
}

void ATeamState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ATeamState, TeamID, COND_InitialOnly);
}

int ATeamState::GetTeam() const
{
	return TeamID;
}

bool ATeamState::IsPlayerOnTeam(ADefaultPlayerState* InPlayerState) const
{
	return Players.Find(InPlayerState) > INDEX_NONE;
}

int32 ATeamState::GetCurrentPlayerNum() const
{
	return Players.Num();
}

void ATeamState::SetTeam(const int InTeamID)
{
	if (HasAuthority())
	{
		TeamID = InTeamID;
		OnRep_TeamID();
	}
}

void ATeamState::LoadServerDefaults(ADefaultMode* InGameMode)
{

}


void ATeamState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UWorld* world = GetWorld();
	ADefaultGameState* gs = world->GetGameState<ADefaultGameState>();
	if (!HasAuthority() && IsValid(gs))
	{
		gs->OnLocalTeamStateRecieved(this);
	}
}
