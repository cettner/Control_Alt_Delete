// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "RTS_Project/GameArchitecture/Game/DefaultHUD.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSFPSWidget.h"
#include "RTSFPSHUD.generated.h"

/**
 * 
 */

enum HUDSTATE 
{
	LBOUND,
	GAME_INIT,
	RTS_SELECT_AND_MOVE,
	RTS_STRUCTURE_SELECT,
	FPS_AIM_AND_SHOOT,
	FPS_AWAITING_RESPAWN,
	UBOUND
};


UCLASS()
class RTS_PROJECT_API ARTSFPSHUD : public ADefaultHUD
{
	GENERATED_BODY()

public:
	ARTSFPSHUD();
	virtual HUDSTATE GetHUDState() const;
	virtual void ChangeHUDState(HUDSTATE statetype);
	virtual FVector2D GetMouseLocation() const;

protected:
	virtual bool InitPrimaryUI() override;

protected:
	virtual void RTSSelectAndMoveHandler();
	virtual void RTSStructureSelectHandler();
	virtual void FPSAimAndShootHandler();

protected:
	virtual void DrawHUD() override;  // HUD "tick" function

protected:

	HUDSTATE state;

};
