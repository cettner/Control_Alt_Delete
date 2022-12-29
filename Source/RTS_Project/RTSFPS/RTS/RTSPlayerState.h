// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"
#include "RTSPlayerController.h"
#include "RTSPlayerState.generated.h"






UCLASS()
class RTS_PROJECT_API ARTSPlayerState : public ARTFPSPlayerState
{
	GENERATED_BODY()
	
	friend ARTSPlayerController;

	protected:
		virtual void InitEnemySelection();

};
