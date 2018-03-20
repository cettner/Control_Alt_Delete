// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RTSMinion.h"
#include "RTSHUD.generated.h"


/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void DrawHUD() override;  // HUD "tick" function

	FVector2D Initial_select;  // intial mouse cursor location
	FVector2D End_Select;		//mouse cursor location on release
	
	FVector2D GetMouseLocation();

	TArray <ARTSMinion*> Selected_Units;

	bool SelctionInProcess = false;

	UPROPERTY(EditAnywhere)
		float selection_transparency = 0.15f;
		
};
