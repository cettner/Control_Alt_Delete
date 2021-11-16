// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "RTS_Project/GameArchitecture/Game/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"
#include "FPSPlayerState.generated.h"


USTRUCT()
struct FUpgradeInfo
{
	GENERATED_USTRUCT_BODY()
public:
	/*Class Of The Upgrade*/
	TSubclassOf<UUpgrade> UpgradeClass = nullptr;

	/*Number of Times the Upgrade Has Been Applied*/
	int Rank = 0;

	/*For Comparision, We only care that the class is the same,  That way, if we want to "Find" it in an array
	we can simulate a "MultiMap" Style Behavior Between the upgrade and its Rank*/
	friend bool operator == (const FUpgradeInfo& Myself, const FUpgradeInfo& Other)
	{
		bool isSame = Myself.UpgradeClass == Other.UpgradeClass;
		return(isSame);
	}

	friend bool operator == (const FUpgradeInfo& Myself, const TSubclassOf<UUpgrade>& Other)
	{
		bool isSame = Myself.UpgradeClass == Other;
		return(isSame);
	}
};


enum EPlayerReswpawnState
{
	PREGAME,
	ALIVE,
	SELECTINGRESPAWN,
	AWAITINGRESPAWN,
};


UCLASS()
class RTS_PROJECT_API AFPSPlayerState : public ARTFPSPlayerState, public IUpgradableInterface
{
	GENERATED_BODY()

public:
		
	EPlayerReswpawnState GetRespawnState() const;
	void SetRespawnState(EPlayerReswpawnState NewState);

public:
	virtual bool AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd) override;
	virtual TArray<TSubclassOf<UUpgrade>> GetAppliedUpgrades() const override;

protected:
	TArray<FUpgradeInfo> AppliedUpgrades;

	EPlayerReswpawnState RespawnState = EPlayerReswpawnState::PREGAME;

};