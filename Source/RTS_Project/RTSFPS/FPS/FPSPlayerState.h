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
		void StoreAppliedUpgrade(TSubclassOf<UUpgrade> AppliedUpgrade);
		
		EPlayerReswpawnState GetRespawnState() const;
		void SetRespawnState(EPlayerReswpawnState NewState);

	protected:
		TArray<FUpgradeInfo> AppliedUpgrades;

		EPlayerReswpawnState RespawnState = EPlayerReswpawnState::PREGAME;

};
