// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultGameState.h"
#include "RTFPSGameState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTFPSGameState : public ADefaultGameState
{
	GENERATED_BODY()

	public:
		ARTFPSGameState(const FObjectInitializer & FOI);
		int NumRTSPlayers(int Team_Index);

	protected:
		virtual void BeginPlay() override;



};