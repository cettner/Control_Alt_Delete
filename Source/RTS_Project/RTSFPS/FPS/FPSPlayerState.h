// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Curves/CurveFloat.h"

#include "RTS_Project/GameArchitecture/Game/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
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
};


enum EPlayerReswpawnState
{
	PREGAME,
	ALIVE,
	SELECTINGRESPAWN,
	AWAITINGRESPAWN,
};


UCLASS()
class RTS_PROJECT_API AFPSPlayerState : public ARTFPSPlayerState, public IUpgradableInterface, public IExpAccumulatorInterface
{
	GENERATED_BODY()

public:
		
	EPlayerReswpawnState GetRespawnState() const;
	void SetRespawnState(EPlayerReswpawnState NewState);

/************UpgradabaleInterface**********/
public:
	virtual bool AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd) override;
	virtual TArray<TSubclassOf<UUpgrade>> GetAppliedUpgrades() const override;
/******************************************/

	/************UpgradabaleInterface**********/
public:
	virtual uint32 GetCurrentExp() const override;
	virtual uint32 GetMaxExpForLevel() const override;
	virtual bool CanRecieveExp() const override;
	virtual uint32 GetCurrentLevel() const override;
	virtual uint32 GetMaxLevel() const override;
	virtual uint32 GetExptoNextLevel() const override;

public:
	/*Primary External Entry Point*/
	virtual void GrantExp(uint32 inexp) override;

protected:
	virtual void OnLevelUp() override;
	/******************************************/

	/******************Actor*******************/
protected:
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	/******************************************/

	/************RunTime Data*************/
protected:
	TArray<FUpgradeInfo> AppliedUpgrades;

	EPlayerReswpawnState RespawnState = EPlayerReswpawnState::PREGAME;

	UPROPERTY(Replicated)
	uint32 TotalUpgradePoints = 0U;

	UPROPERTY(Replicated)
	uint32 SpentUpgradePoints = 0U;

	UPROPERTY(Replicated)
	uint32 CurrentExperiance = 0U;

	UCurveFloat * ExpCurve;
	/*************************************/
};
