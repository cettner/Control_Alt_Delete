// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMode.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTS_Project/RTSFPS/GameObjects/Resource.h"
#include "RTFPSMode.generated.h"





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
	virtual void InitializeDeferredDefaultPawn(APawn * DefferedPawn, AController * InheritingController) override;
	virtual void StartMatch() override;


public:
	virtual int GetStartingResources(TSubclassOf<AResource> ResourceClass);
	TArray<TSubclassOf<AResource>> GetResourceTypes() const;
	TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetDefaultUnitCosts() const;


protected:
	int MaxRTSPlayersPerTeam = 1;
	int MinRTSPlatersPerTeam = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int StartingResource = 100;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AResource>> ResourceTypes;

	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "RTSObjectInterface"))
	TMap<TSubclassOf<UObject>, FReplicationResourceMap> DefaultUnitCosts;

};
