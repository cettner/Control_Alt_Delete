// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerState.h"

EPlayerReswpawnState AFPSPlayerState::GetRespawnState() const
{
	return RespawnState;
}

void AFPSPlayerState::SetRespawnState(EPlayerReswpawnState NewState)
{
	RespawnState = NewState;
}
