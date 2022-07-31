// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UI/UpgradeTreeWidget.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultHUD.h"
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

enum ETalentTreeTypes
{
	AUTO,
	RTS,
	FPS
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
	virtual bool TryToggleUpgradeTree(ETalentTreeTypes InTreeToToggle = ETalentTreeTypes::AUTO);
	virtual void RefreshUpgradeTree();

protected:
	virtual bool ClientInitializeHUD() override;
	virtual FStackWidgetInfo GetDefaultInputSettings() const override;

protected:
	virtual void RTSSelectAndMoveHandler();
	virtual void RTSStructureSelectHandler();
	virtual void FPSAimAndShootHandler();

protected:
	virtual void DrawHUD() override;  // HUD "tick" function

protected:
	/*Adding a new default input to handle FPS Players RTS Is Handeled In Base Class*/
	UPROPERTY(EditDefaultsOnly)
	FStackWidgetInfo FPSDefaultInput;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUpgradeTreeWidget> FPSTalentTreeClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUpgradeTreeWidget> RTSTalentTreeClass;



protected:
	HUDSTATE state;

	UPROPERTY()
	UUpgradeTreeWidget * FPSUpgradeTree = nullptr;

	UPROPERTY()
	UUpgradeTreeWidget * RTSUpgradeTree = nullptr;

};
