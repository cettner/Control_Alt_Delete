// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/TeamMultiplayerGame/Game/TeamPlayerStart.h"
#include "RTFPSPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTFPSPlayerStart : public ATeamPlayerStart
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		bool isRTSStart = false;
	
	
	
};
