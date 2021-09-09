// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DefaultGameState.h"
#include "DefaultHUD.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ADefaultHUD : public AHUD
{
	GENERATED_BODY()

	public:
		virtual bool InitializeUI();
};
