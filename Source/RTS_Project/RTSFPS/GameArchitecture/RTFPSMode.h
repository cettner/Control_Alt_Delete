// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultMode.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Resource.h"
#include "RTFPSMode.generated.h"





UCLASS()
class RTS_PROJECT_API ARTFPSMode : public ADefaultMode
{
	GENERATED_BODY()

public:
	ARTFPSMode(const FObjectInitializer& ObjectInitializer);

public:



protected:
	virtual UClass * GetDefaultPawnClassForController_Implementation(AController * InController) override;
	virtual AActor * FindPlayerStart_Implementation(AController * Player, const FString & IncomingName) override;
	virtual bool FinishPlayerRegistration(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings) override;
	virtual void InitializeDeferredDefaultPawn(APawn * DefferedPawn, AController * InheritingController) override;
	virtual void StartMatch() override;
	virtual bool ReadyToEndMatch_Implementation() override;

protected:
	/** The default pawn class used by RTS players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ARTSCamera> DefaultRTSClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ACommander> DefaultFPSClass = nullptr;

	/********************RTS Initialization*********************/
public:
	TSubclassOf<ARTSCamera> GetDefaultRTSClass() const;
	FReplicationResourceMap GetStartingResources() const;
	TArray<TSubclassOf<AResource>> GetResourceTypes() const;
	TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetDefaultUnitCosts() const;
	/***********************************************************/

public:
	/********************FPS Initialization*********************/
	TSubclassOf<ACommander> GetDefaultFPSClass() const;
	UCurveFloat* GetExpCurve() const;
	uint32 GetMaxLevel() const;
	/**********************************************************/
protected:
	
	/********************RTS Initialization*********************/
	UPROPERTY(EditDefaultsOnly, Category = "RTS Initialization")
	FReplicationResourceMap StartingResource;
	
	UPROPERTY(EditDefaultsOnly, Category = "RTS Initialization")
	TArray<TSubclassOf<AResource>> ResourceTypes;

	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "RTSObjectInterface"), Category = "RTS Initialization")
	TMap<TSubclassOf<UObject>, FReplicationResourceMap> DefaultUnitCosts;
	/***********************************************************/

	/********************FPS Initialization*********************/
	UPROPERTY(EditDefaultsOnly, Category = "FPS Initialization")
	UCurveFloat * ExpCurve = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = "FPS Initialization")
	uint32 MaxLevel = 10U;
	/**********************************************************/
};
