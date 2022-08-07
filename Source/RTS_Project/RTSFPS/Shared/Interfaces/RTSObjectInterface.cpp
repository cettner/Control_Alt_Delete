// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSObjectInterface.h"

// Add default functionality here for any IRTSObjectInterface functions that are not pure virtual.

void IRTSObjectInterface::SetSelected()
{
}

void IRTSObjectInterface::SetDeselected()
{
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

bool IRTSObjectInterface::IsAlive() const
{
	return false;
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
