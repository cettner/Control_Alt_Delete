// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSCamera.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTSSelectionCamera.generated.h"

/**
 * 
 */
#define SELECTION_CHANNEL  ECC_GameTraceChannel1

UCLASS()
class RTS_PROJECT_API ARTSSelectionCamera : public ARTSCamera
{
	GENERATED_BODY()
	
	public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSMinion*> SelectedUnits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray <ARTSStructure*> SelectedStructures;

	private:
	void SelectPressed();
	void SelectReleased();
	void MoveSelected();
	
	
};
