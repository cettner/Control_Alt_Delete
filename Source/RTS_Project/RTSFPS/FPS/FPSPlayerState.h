// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Curves/CurveFloat.h"

#include "RTS_Project/RTSFPS/RTS/RTSPlayerState.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "FPSPlayerState.generated.h"


enum EPlayerReswpawnState
{
	PREGAME,
	ALIVE,
	SELECTINGRESPAWN,
	AWAITINGRESPAWN,
};


UCLASS()
class RTS_PROJECT_API AFPSPlayerState : public ARTSPlayerState, public IUpgradableInterface, public IExpAccumulatorInterface
{
	GENERATED_BODY()

	AFPSPlayerState();

public:
	EPlayerReswpawnState GetRespawnState() const;
	void SetRespawnState(EPlayerReswpawnState NewState);
	void SetTotalUpgradePoints(uint32 InTotalUpgradePoints);
	void SetSpentUpgradePoints(uint32 InSpentUpgradePoints);
	FORCEINLINE UUpgradeData* GetUpgradeData() const { return UpgradeData; }

protected:
	UFUNCTION()
	virtual void OnRep_TotalUpgradePoints();

	UFUNCTION()
	virtual void OnRep_SpentUpgradePoints();

	UFUNCTION()
	virtual void OnRep_AppliedUpgrades();
/************UpgradabaleInterface**********/
public:
	virtual TArray<TSubclassOf<UUpgrade>> GetAllUpgrades() const override;
	virtual uint32 GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const override;
	virtual bool IncrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToUnLearn) override;
	virtual bool DecrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToUnLearn) override;
/******************************************/

	/************ExpAccumulatorInterface**********/
public:
	virtual uint32 GetCurrentExp() const override;
	virtual uint32 GetMaxExpForCurrentLevel() const override;
	virtual bool CanRecieveExp() const override;
	virtual uint32 GetCurrentLevel() const override;
	virtual uint32 GetMaxLevel() const override;
	virtual int32 GetExptoNextLevel() const override;
	virtual uint32 GetExpforLevel(uint32 inLevel) const override;
	
	virtual uint32 GetAvailableUpgradePoints() const override;
	virtual uint32 GetSpentUpgradePoints() const override;
	virtual uint32 GetTotalUpgradePoints() const override;

public:
	/*External Functions*/
	virtual void GrantExp(uint32 inexp) override;
	virtual bool SpendUpgradePoints(uint32 PointsToSpend = 1U) override;

protected:
	virtual void OnLevelUp() override;
	/******************************************/

	/****************DefaultPlayerState********/
protected:
	virtual void LoadGameModeDefaults(const AGameModeBase* GameModeCDO) override;
	/******************************************/
	/******************Actor*******************/
protected:
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	/******************************************/

	/************RunTime Data*************/
protected:

	EPlayerReswpawnState RespawnState = EPlayerReswpawnState::PREGAME;

	UPROPERTY(ReplicatedUsing = OnRep_AppliedUpgrades)
	UUpgradeData * UpgradeData = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_TotalUpgradePoints)
	uint32 TotalUpgradePoints = 0U;

	UPROPERTY(ReplicatedUsing = OnRep_SpentUpgradePoints)
	uint32 SpentUpgradePoints = 0U;

	UPROPERTY(Replicated)
	uint32 CurrentExperiance = 0U;

	uint32 MaxLevel = 0U;

	UCurveFloat* ExpCurve = nullptr;
	/*************************************/
};
