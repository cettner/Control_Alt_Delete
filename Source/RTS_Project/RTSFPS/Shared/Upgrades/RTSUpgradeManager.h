// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "../../RTS/RTSMinion.h"
#include "../../GameSystems/UpgradeSystem/UpgradeData.h"
#include "RTSUpgradeManager.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSUpgradeManager : public AInfo
{
	GENERATED_BODY()

public:
	void SetTeamID(int32 InTeamID);
	FORCEINLINE int32 GetTeamID() const { return TeamID; }

protected:
	UFUNCTION()
	virtual void OnRep_TeamID();

	virtual void RegisterMinion(ARTSMinion* InMinion);
	virtual void UnRegisterMinion(ARTSMinion* InMinion);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


protected:
	UPROPERTY(ReplicatedUsing = OnRep_TeamID)
	int32 TeamID = -1;

	TMap<TSubclassOf<ARTSMinion>, int32> IndexMapping = TMap<TSubclassOf<ARTSMinion>, int32>();

	UPROPERTY(Replicated)
	TArray<UUpgradeData*> Upgrades = TArray<UUpgradeData*>();
};
