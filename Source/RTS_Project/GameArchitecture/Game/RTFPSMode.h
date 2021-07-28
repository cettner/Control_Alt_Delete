// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMode.h"
#include "../../RTSFPS/FPS/Commander.h"
#include "../../RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTS_Project/RTSFPS/GameObjects/Resource.h"
#include "RTFPSMode.generated.h"



USTRUCT()
struct FReplicationResourceMap
{
	GENERATED_USTRUCT_BODY()
public:
	void Emplace(TSubclassOf<AResource> Key, int Value)
	{
		int index = Keys.IndexOfByKey(Key);

		if (index == INDEX_NONE)
		{
			Keys.Emplace(Key);
			Values.Emplace(Value);
		}
		else
		{
			Values[index] = Value;
		}
	}
	const int* Find(TSubclassOf<AResource> Key) const
	{
		int index = Keys.IndexOfByKey(Key);

		if (index != INDEX_NONE)
		{
			const int * retval = &Values[index];
			return(retval);
		}

		return(nullptr);
	}
	int Num() const
	{
		return(Keys.Num());
	}
	TMap<TSubclassOf<AResource>, int> GetMap() const
	{
		TMap<TSubclassOf<AResource>, int> Map = TMap<TSubclassOf<AResource>, int>();
		if (IsValid())
		{
			for (int i = 0; i < Keys.Num(); i++)
			{
				Map.Emplace(Keys[i], Values[i]);
			}
		}


		return(Map);
	}
	bool IsValid() const
	{
		return(Keys.Num() == Values.Num());
	}

protected:
	UPROPERTY(EditDefaultsOnly)
		TArray<int> Values;

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AResource>> Keys;
};


UCLASS()
class RTS_PROJECT_API ARTFPSMode : public ADefaultMode
{
	GENERATED_BODY()

public:
	ARTFPSMode(const FObjectInitializer& ObjectInitializer);

public:
	/** The default pawn class used by RTS players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ARTSCamera> DefaultRTSClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ACommander> DefaultFPSClass = nullptr;

protected:
	virtual UClass * GetDefaultPawnClassForController_Implementation(AController * InController) override;
	virtual AActor * FindPlayerStart_Implementation(AController * Player, const FString & IncomingName) override;
	virtual bool FinishPlayerRegistration(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings) override;
	virtual void StartMatch() override;

public:
	virtual int GetStartingResources(TSubclassOf<AResource> ResourceClass);
	TArray<TSubclassOf<AResource>> GetResourceTypes() const;
	TMap<TSubclassOf<AActor>, FReplicationResourceMap> GetDefaultUnitCosts() const;


protected:
	int MaxRTSPlayersPerTeam = 1;
	int MinRTSPlatersPerTeam = 1;
	
	UPROPERTY(EditDefaultsOnly)
	int StartingResource = 100;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AResource>> ResourceTypes;

	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "RTSObjectInterface"))
	TMap<TSubclassOf<AActor>, FReplicationResourceMap> DefaultUnitCosts;

};
