// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Interfaces/RTSFlockAgentInterface.h"
#include "RTSFlock.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSFlock : public UObject
{
	GENERATED_BODY()



	public:
		virtual void UpdateFlock();
		
		inline const TArray<IRTSFlockAgentInterface*>& GetAgents() const
		{
			return FlockAgents;
		}


	protected:
		TArray<IRTSFlockAgentInterface*> FlockAgents;
	
};
