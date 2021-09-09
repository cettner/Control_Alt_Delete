// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSHUD.h"

ARTSFPSHUD::ARTSFPSHUD() : Super()
{
	MainUIClass = URTSFPSWidget::StaticClass();
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
		
		if (MainUI != nullptr)
		{
			MainUI->OnHUDStateChange();
		}
	}
}

HUDSTATE ARTSFPSHUD::GetHUDState()
{
	return (state);
}

FVector2D ARTSFPSHUD::GetMouseLocation()
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	return(FVector2D(PosX, PosY));
}

bool ARTSFPSHUD::InitializeUI()
{
	Super::InitializeUI();

	if (MainUIClass == nullptr || MainUI != nullptr) return(false);
	MainUI = CreateWidget<URTSFPSWidget>(GetOwningPlayerController(), MainUIClass,"MainUI");

	if (MainUI == nullptr) return(false);
	MainUI->Setup();
	MainUI->AddToViewport();

	return (true);
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
