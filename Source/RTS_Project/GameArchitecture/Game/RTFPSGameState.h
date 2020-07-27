// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultGameState.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTFPSGameState.generated.h"

struct RTSTeamUnits
{
	TArray<ARTSMinion*> Minions = TArray<ARTSMinion*>();
	TArray<ARTSStructure*> Structures = TArray<ARTSStructure*>();
};

UCLASS()
class RTS_PROJECT_API ARTFPSGameState : public ADefaultGameState
{
	GENERATED_BODY()

	public:
		ARTFPSGameState(const FObjectInitializer & FOI);
		int NumRTSPlayers(int Team_Index);
		virtual void RefreshAllUnits();
		virtual void OnMinionDeath(ARTSMinion* Minion);
		virtual void HandlePlayerDeath(AFPSServerController * Controller);

    public:
		virtual bool TeamInitialize(ADefaultMode* GameMode) override;


	protected:
		TArray<RTSTeamUnits> AllUnits;
		RTSTeamUnits InvalidUnits;


};
