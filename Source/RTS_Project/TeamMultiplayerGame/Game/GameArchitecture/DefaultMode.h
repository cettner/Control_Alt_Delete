// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "../TeamPlayerStart.h"
#include "DefaultPlayerController.h"
#include "../../LobbyGameInstance.h"
#include "DefaultMode.generated.h"

/*
 * 
 */

/*Helper Struct*/
struct TeamSpawnSelector
{
public:
	void Add(ATeamPlayerStart * Add) 
	{
		if (Add && LastSpawn < 0)
		{
			LastSpawn = 0;
		}
		TeamStart.AddUnique(Add);
	}
	ATeamPlayerStart * GetNextSpawn()
	{
		ATeamPlayerStart * retval = nullptr;
		if (TeamStart.Num())
		{
			retval = TeamStart[LastSpawn];
			if (LastSpawn + 1 == TeamStart.Num())
			{
				LastSpawn = 0;
			}
			else
			{
				LastSpawn++;
			}

		}
		return(retval);
	}
	int Num() 
	{
		return(TeamStart.Num());
	}

protected:
	int LastSpawn = -1;
	TArray<ATeamPlayerStart *> TeamStart;
};

UCLASS()
class RTS_PROJECT_API ADefaultMode : public AGameMode
{
	GENERATED_BODY()
public:
	ADefaultMode(const FObjectInitializer& ObjectInitializer);
	virtual bool RegisterPlayerData(ADefaultPlayerController * RegisteringPlayer, FPlayerSettings settings);
	int GetNumTeams() { return (NumTeams); }
	int GetTeamSize() { return (TeamSize); }

public:
	virtual void EndMatch() override;

protected:
	virtual void PostInitializeComponents() override;
	virtual AActor * FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	virtual bool ReadyToStartMatch_Implementation() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartMatch() override;


protected:
	virtual bool LoadServerData();
	virtual bool FinishPlayerRegistration(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings);
	virtual FPlayerSettings FetchSettingsFromLobbyData(APlayerController* NewPlayer);
	virtual bool CheckPlayerRegistry();
	virtual void InitializeDeferredDefaultPawn(APawn * DefferedPawn, AController * InheritingController);
	virtual APawn * SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	UFUNCTION()
	virtual void StartReturnPlayersToLobby();
	virtual void ReturnPlayersToLobby();


/*************Config Data*************/
protected:
	UPROPERTY(EditDefaultsOnly)
	float TimeToReturnToLobby = 10.0f;
/************************************/

/*****Loaded from Game Instance******/
protected:
	int NumTeams;
	int TeamSize;
	TArray<FPlayerSettings> LobbyPlayers;
/************************************/
	
/**********************Runtime Data*********************/
protected:
	TMap<TSharedPtr<const FUniqueNetId>, bool> PlayerRegistry;

	TArray<TeamSpawnSelector> TeamStartingPoints;

	FTimerHandle ReturnToLobbyHandle = FTimerHandle();
/*************************************************************************/
	

protected:
	/*Used By Editor Because Game Instance Cannot be Set Properly*/
	UPROPERTY(EditDefaultsOnly, Category = Debug)
	FServerSettings DefaultSettings;

#if WITH_EDITOR
protected:
	virtual FServerSettings GetDefaultSettings() const;

protected:
	/*Editor Only Class used to spoof the registration system into beleiveing editor players are real*/
	class FEditorUniqueNetID : public FUniqueNetId
	{

		/*************************************************************************/
	public:
		FEditorUniqueNetID() { /*Have to Define from parent*/ }

		int32 GetSize() const override
		{
			return ((int32)sizeof(seed));
		}

		const uint8* GetBytes() const override
		{
			return(&seed);
		}

		bool IsValid() const override
		{
			return(bHasBeenSet);
		}

		FString ToString() const override
		{
			FString retval = FString::FromInt(seed);
			return(retval);
		}

		FString ToDebugString() const override
		{
			return(ToString());
		}
		/*************************************************************************/
		void SetSeed(uint8 InSeed)
		{
			seed = InSeed;
			bHasBeenSet = true;
		}

		void InValidate()
		{
			bHasBeenSet = false;
		}


	private:
		uint8 seed = 0U;
		bool bHasBeenSet = false;
	};

	FPlayerSettings EditorFetchPlayerSettings(APlayerController* Controller);
	TSharedPtr<const FUniqueNetId> EditorCreatePlayerID();

private:
	int EditorPlayerCount = 0;
#endif
};
