// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSObjectInterface.h"

// Add default functionality here for any IRTSObjectInterface functions that are not pure virtual.

void IRTSObjectInterface::SetSelected()
{
}

void IRTSObjectInterface::SetDeselected()
{
}

bool IRTSObjectInterface::IsBoxSelectable() const
{
	return false;
}

int IRTSObjectInterface::GetTeam() const
{
	return -1;
}

void IRTSObjectInterface::SetTeam(int teamid)
{
}

void IRTSObjectInterface::SetTeamColors(FLinearColor TeamColor)
{
}

void IRTSObjectInterface::OnLocalPlayerTeamChange(int InLocalTeamID)
{
}

bool IRTSObjectInterface::IsAlive() const
{
	return false;
}

bool IRTSObjectInterface::IsRTSObjectValid(const IRTSObjectInterface* InRTSObject)
{
	return IsValid(Cast<UObject>(InRTSObject));
}

void IRTSObjectInterface::SetTarget(AActor* InNewTarget)
{
}

void IRTSObjectInterface::SetTargetLocation(FVector InTargetLocation)
{
}

AActor* IRTSObjectInterface::GetTarget()
{
	return nullptr;
}

void IRTSObjectInterface::ClearTarget()
{
}

IRTSObjectInterface* IRTSObjectInterface::GetLeadRTSObject()
{
	return nullptr;
}
