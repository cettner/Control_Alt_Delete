// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "RTSOrderGroup.h"

#include "RTSOrderManager.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSOrderManager : public AInfo
{
	GENERATED_BODY()

	friend URTSOrderGroup;

public:
	URTSOrderGroup* BuildOrderGroup(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits,const FHitResult& InHitContext, AController* Issuer);
	URTSOrderGroup* GetGroupByID(const uint32 InID) const;
	void OnGroupEmptied(URTSOrderGroup* EmptyGroup);

protected:
	FORCEINLINE uint32 GenerateNextGroupID() 
	{ 
		uint32 retval = NextGroupID;
		NextGroupID++;
		return retval;
	};

protected:
	UPROPERTY()
	TMap<uint32, URTSOrderGroup*> OrderGroups = TMap<uint32, URTSOrderGroup*>();

	uint32 NextGroupID = 0;
};
