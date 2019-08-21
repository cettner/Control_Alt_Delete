// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTSCamera.h"
#include "RTSAIController.h"
#include "Components/InputComponent.h"
#include "RTSSelectable.h"
#include "RTSSpawnHelper.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */

#define NUM_RESOURCES 3
#define RESOURCE_ONE_PLAYER_START 200
#define RESOURCE_TWO_PLAYER_START 150
#define RESOURCE_THREE_PLAYER_START 0
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
class RTS_PROJECT_API ARTSPlayerController : public APlayerController
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

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ARTSStructure * Spawn_RTS_Structure(FVector Location, FRotator Rotation, int Structure_index);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	ARTSMinion * Spawn_RTS_Minion(FVector Location, FRotator Rotation, int Unit_index);

	UFUNCTION(BlueprintCallable, Category = Spawn)
	AWeapon * Spawn_Weapon(FVector Location, FRotator Rotation, int Weapon_index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> SelectedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSStructure*> SelectedStructures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <int> Resource_Count;

	UFUNCTION(Server, unreliable,  WithValidation)
	void PossessCommander(ACommander * commander);

	UFUNCTION(Server, reliable, WithValidation)
	void PossessRTSCamera(ARTSCamera * camera);

	UFUNCTION(Server, unreliable, WithValidation)
	void MoveMinions(ARTSPlayerController * PC, const TArray<ARTSMinion *>& Units, FHitResult Hit);

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(Server, unreliable, WithValidation)
	void FinishPlayerLogin();
	

private:

	RTSSpawnHelper  SpawnHelper;
};
