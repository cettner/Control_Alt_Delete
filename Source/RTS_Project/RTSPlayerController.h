// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTSCamera.h"
#include "RTSBUILDER.h"
#include "RTSAIController.h"
#include "Components/InputComponent.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */

#define NUM_RESOURCES 3
#define RESOURCE_ONE_PLAYER_START 200
#define RESOURCE_TWO_PLAYER_START 150
#define RESOURCE_THREE_PLAYER_START 0
#define MAX_RESOURCES 9999

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
	



	ARTSHUD * HudPtr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> SelectedUnits;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSStructure*> SelectedStructures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int ResourceOnePLayerStart = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int ResourceTwoPLayerStart = 150;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Gameplay)
		int ResourceThreePLayerStart = 0;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void AddResource(int amount_to_add, Resource_Types type);
	bool TakeResource(int amount_to_take, Resource_Types type);
	
protected:

	void SelectPressed();
	void SelectReleased();
	void MoveSelected();
	void SwapHud();

private:
	TArray <int> Resource_Count;
};
