// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Navigation/RTSFlock.h"
#include "Navigation/Interfaces/RTSFlockAgentInterface.h"
#include "RTSNavManager.generated.h"


UCLASS()
class RTS_PROJECT_API URTSNavManager : public UObject
{
	friend URTSFlock;
	
	GENERATED_BODY()
	


protected:
	inline const TArray<AActor*>& GetAllActiveAgents() const
	{
		return AllAgents;
	}

protected:
	TArray<AActor*> AllAgents = TArray<AActor*>();
	TArray<URTSFlock*> Flocks = TArray<URTSFlock*>();
};
