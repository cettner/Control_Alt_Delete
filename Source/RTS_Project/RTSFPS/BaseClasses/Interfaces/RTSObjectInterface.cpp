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
	return 0;
}

void IRTSObjectInterface::SetTeam(int teamid)
{
}

void IRTSObjectInterface::SetTeamColors(FLinearColor TeamColor)
{
}
