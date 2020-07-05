// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "Components/InputComponent.h"
#include "RTSFPS/RTS/Camera/RTSSelectable.h"
#include "RTSFPS/RTS/FOW/FogOfWarManager.h"
#include "GameArchitecture/Game/RTFPSPlayerState.h"
#include "GameArchitecture/Game/DefaultPlayerController.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */

#define NUM_RESOURCES 3
#define MAX_RESOURCES 9999
#define SELECTION_CHANNEL  ECC_GameTraceChannel1

enum Resource_Types
{
	TYPE_ONE,
	TYPE_TWO,
	TYPE_THREE,
	NULL_TYPE
};

class ARTSStructure;
class AWeapon;

UCLASS()
class RTS_PROJECT_API ARTSPlayerController : public ADefaultPlayerController
{
	GENERATED_BODY()
	
public:
	ARTSPlayerController();
	
	UFUNCTION(BlueprintImplementableEvent, Category = UI)
		void Update_UI_Selection();

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
		void Update_UI_Resource();

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
		void Update_UI_Spawn(AActor * NewSpawn);

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

	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;

	void FinishLocalPlayerSetup(ARTFPSPlayerState * PS);
	
	virtual AFogOfWarManager * InitFOW();
	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fog Of War")
	TSubclassOf<AFogOfWarManager> FOWManagerClass;
	AFogOfWarManager * FOWManager;

private:
	void DebugEvent();

};
