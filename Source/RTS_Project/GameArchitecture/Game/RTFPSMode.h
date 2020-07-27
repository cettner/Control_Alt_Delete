// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMode.h"
#include "../../RTSFPS/FPS/Commander.h"
#include "../../RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTFPSMode.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTFPSMode : public ADefaultMode
{
	GENERATED_BODY()

public:
	ARTFPSMode(const FObjectInitializer& ObjectInitializer);

public:
	/** The default pawn class used by RTS players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ARTSCamera> DefaultRTSClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ACommander> DefaultFPSClass = nullptr;

protected:
	virtual UClass * GetDefaultPawnClassForController_Implementation(AController * InController) override;
	virtual AActor * FindPlayerStart_Implementation(AController * Player, const FString & IncomingName) override;
	virtual bool FinishPlayerRegistration(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings) override;
	virtual void StartMatch() override;

protected:
	int MaxRTSPlayersPerTeam = 1;
	int MinRTSPlatersPerTeam = 1;
	
};
