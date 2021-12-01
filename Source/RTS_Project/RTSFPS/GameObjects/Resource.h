// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridClaimingActor.h"


#include "Components/StaticMeshComponent.h"
#include "Resource.generated.h"

USTRUCT(Blueprintable)
struct FResourceUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName ResourceName = "";

	UPROPERTY(EditDefaultsOnly)
	UTexture* ResourceImage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AResource> Key = nullptr;
};

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

	/*Allows Pair Iteration*/
	TPair<TSubclassOf<AResource>, int > operator[](int Index)
	{
		return TPair<TSubclassOf<AResource>, int >(Keys[Index], Values[Index]);
	}
protected:
	UPROPERTY(EditDefaultsOnly)
		TArray<int> Values;

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AResource>> Keys;
};


UCLASS(Blueprintable)
class RTS_PROJECT_API AResource : public AGridClaimingActor, public IRTSObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource();
	uint32 Mine(uint32 amount_to_mine);
	FResourceUIData GetUIData() const;
	uint32 GetResourceWeight() const;

public:
	/*RTSObject Interface Overrides*/
	virtual void SetSelected() override;
	virtual void SetDeselected() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	UDecalSelectionComponent* SelectionComp;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	UStaticMeshComponent* MeshComp;

protected:

	UPROPERTY(EditAnywhere, Category = Gameplay)
	bool bisinfinite = false;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	uint32 ResourceVal = 90;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	uint32 ResourceWeight = 1U;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	FResourceUIData UIData;
};
