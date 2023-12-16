// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "..\..\GameSystems\ResourceSystem\MineableResource.h"
#include "RTSTargetedOrder.h"
#include "RTSMineResourceOrder.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSMineResourceOrder : public URTSTargetedOrder
{
	GENERATED_BODY()
	protected:
		virtual void SetTargetContext(AController* Issuer, const FHitResult& InContext) override;
		virtual void LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const override;

	public:
		const FName& GetResourceNodeKey()const {return ResourceNodeKey; }
		const FName& GetResourceClassKey() const { return ResourceClassKey; }
		const FName& GetLastMinedLocationKey() const { return LastMinedLocationKey; }

	protected:
		/*The inital Resource that was selected for harvesting, this can change over the course of the order in the Behavior tree*/
		TWeakObjectPtr<AMineableResource> StartNode = nullptr;
		
		/*The location of the Resource that was initally ordered, may be used as reference if the node is destroyed prior to reaching it*/
		FVector StartNodeLocation = FVector();
		
		/*The Class of Resource to Mine*/
		TSubclassOf<UResource> ResourceClassToMine = nullptr;

	public:
		static const FName ResourceNodeKey;
		static const FName ResourceClassKey;
		static const FName LastMinedLocationKey;
		static const FName ResourceDroppointKey;
};
