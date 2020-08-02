// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
class RTS_PROJECT_API ARTSFPSHUD : public AHUD
{
	GENERATED_BODY()

public:
	ARTSFPSHUD();
	virtual HUDSTATE GetHUDState();
	virtual void ChangeHUDState(HUDSTATE statetype);
	virtual FVector2D GetMouseLocation();
	virtual bool InitializeUI();

protected:
	virtual void RTSSelectAndMoveHandler();
	virtual void RTSStructureSelectHandler();
	virtual void FPSAimAndShootHandler();

protected:
	virtual void DrawHUD() override;  // HUD "tick" function

protected:
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> MainUIClass;
	URTSFPSWidget* MainUI;

	HUDSTATE state;

};
