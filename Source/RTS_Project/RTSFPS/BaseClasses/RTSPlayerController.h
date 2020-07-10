// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "../RTS/Camera/RTSCamera.h"
#include "../RTS/Camera/RTSSelectable.h"
#include "../RTS/FOW/FogOfWarManager.h"
#include "../../GameArchitecture/Game/DefaultPlayerController.h"
#include "../../GameArchitecture/Game/RTFPSPlayerState.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */

#define NUM_RESOURCES 3
#define SELECTION_CHANNEL  ECC_GameTraceChannel1



class ARTSStructure;
class AWeapon;

UCLASS()
class RTS_PROJECT_API ARTSPlayerController : public ADefaultPlayerController
{
	GENERATED_BODY()
	
public:
	ARTSPlayerController();
	
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void UpdateUISelection();

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void UpdateUIResource();

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void UpdateUISpawn(AActor * NewSpawn);

	ARTSHUD * HudPtr;


/**************************LOCAL DATA**************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> SelectedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSStructure*> SelectedStructures;
/*************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <int> Resource_Count;

	UFUNCTION(Server, reliable,  WithValidation)
	void PossessCommander(ACommander * commander);

	UFUNCTION(Server, reliable, WithValidation)
	void PossessRTSCamera(ARTSCamera * camera);

	UFUNCTION(Server, reliable, WithValidation)
	void MoveMinions(ARTSPlayerController * PC, const TArray<ARTSMinion *>& Units, FHitResult Hit);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void ClientNotifyTeamChange(int newteam) override;

	void FinishLocalPlayerSetup(ARTFPSPlayerState * PS);
	
	virtual AFogOfWarManager * InitFOW();
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fog Of War")
	TSubclassOf<AFogOfWarManager> FOWManagerClass;
	AFogOfWarManager * FOWManager;

private:
	void DebugEvent();

};
