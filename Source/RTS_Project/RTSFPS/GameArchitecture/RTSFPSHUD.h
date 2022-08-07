// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UI/UpgradeTreeWidget.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultHUD.h"
#include "RTSFPSWidget.h"
#include "RTSFPSHUD.generated.h"

/**
 * 
 */



enum EGenrePlayType
{
	AUTO,
	RTS,
	FPS,
	NONE
};

UCLASS()
class RTS_PROJECT_API ARTSFPSHUD : public ADefaultHUD
{
	GENERATED_BODY()

public:
	ARTSFPSHUD();
	virtual EGenrePlayType GetGenrePlayType() const;
	virtual void SetGenrePlayType(EGenrePlayType InGenreType);
	virtual bool TryToggleUpgradeTree(EGenrePlayType InTreeToToggle = EGenrePlayType::AUTO);
	virtual void RefreshUpgradeTree();

protected:
	virtual bool ClientInitializeHUD() override;
	virtual FStackWidgetInfo GetDefaultInputSettings() const override;

  // HUD "tick" function

protected:
	/*Adding a new default input to handle FPS Players RTS Is Handeled In Base Class*/
	UPROPERTY(EditDefaultsOnly)
	FStackWidgetInfo FPSDefaultInput;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUpgradeTreeWidget> FPSTalentTreeClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUpgradeTreeWidget> RTSTalentTreeClass;



protected:

	UPROPERTY()
	UUpgradeTreeWidget * FPSUpgradeTree = nullptr;

	UPROPERTY()
	UUpgradeTreeWidget * RTSUpgradeTree = nullptr;



};
