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

	UFUNCTION(BlueprintCallable, Category = HUD)
		void Spawn_RTS_Structure(FVector Location, FRotator Rotation, int Structure_index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> SelectedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSStructure*> SelectedStructures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <int> Resource_Count;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int ResourceOnePLayerStart = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int ResourceTwoPLayerStart = 150;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int ResourceThreePLayerStart = 0;

	UFUNCTION(BlueprintCallable, Category = Setup)
	void PossessCommander(ACommander * commander);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void PossessRTSCamera(ARTSCamera * camera);

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void AddResource(int amount_to_add, Resource_Types type);
	bool TakeResource(int amount_to_take, Resource_Types type);

	void SelectPressed();
	void SelectReleased();
	void MoveSelected();

	RTSSpawnHelper SpawnHelper = RTSSpawnHelper();

private:
	ARTSSelectable * TempClick = nullptr;
	class ViewActor
	{
	public:
		ARTSSelectable * Selectable;
		ARTSMinion * Minion;
		ViewActor()
		{
			Selectable = nullptr;
			Minion = nullptr;
		}
		void empty() { Selectable = nullptr; Minion = nullptr; }
		void set(ARTSSelectable * inselect) {
			Selectable = inselect;
			Minion = nullptr;
		};
		void set(ARTSMinion * inMinion)
		{
			Minion = inMinion;
			Selectable = nullptr;
		}
	};
	ViewActor CurrentView = ViewActor();
};
