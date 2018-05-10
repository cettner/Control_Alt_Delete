// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARTSPlayerController();
	
	ARTSHUD * HudPtr;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
protected:

	void SelectPressed();
	void SelectReleased();
	void MoveSelected();

	TArray <ARTSMinion*> SelectedUnits;
};
