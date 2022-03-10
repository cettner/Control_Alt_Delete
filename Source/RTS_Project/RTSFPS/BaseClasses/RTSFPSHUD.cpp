// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSHUD.h"

ARTSFPSHUD::ARTSFPSHUD() : Super()
{

}

void ARTSFPSHUD::DrawHUD() //similiar to "tick" of actor class overridden
{
	switch (state)
	{
	case HUDSTATE::GAME_INIT:
		break;
	case HUDSTATE::RTS_SELECT_AND_MOVE:
		RTSSelectAndMoveHandler();
		break;
	case HUDSTATE::FPS_AIM_AND_SHOOT:
		FPSAimAndShootHandler();
		break;
	case HUDSTATE::RTS_STRUCTURE_SELECT:
		RTSStructureSelectHandler();
		break;
	default:
		break;
	}

}

void ARTSFPSHUD::ChangeHUDState(HUDSTATE statetype)
{
	if (statetype > LBOUND&& statetype < UBOUND)
	{
		state = statetype;
		URTSFPSWidget* mainui = GetPrimaryUI<URTSFPSWidget>();
		mainui->OnHUDStateChange();
	}
}

HUDSTATE ARTSFPSHUD::GetHUDState() const
{
	return (state);
}

FVector2D ARTSFPSHUD::GetMouseLocation() const
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	return(FVector2D(PosX, PosY));
}

bool ARTSFPSHUD::InitPrimaryUI()
{
	bool retval = Super::InitPrimaryUI();
	URTSFPSWidget* mainui = GetPrimaryUI<URTSFPSWidget>();
	mainui->Setup();

	return retval;
}

void ARTSFPSHUD::RTSSelectAndMoveHandler()
{
}

void ARTSFPSHUD::RTSStructureSelectHandler()
{
}

void ARTSFPSHUD::FPSAimAndShootHandler()
{
}
