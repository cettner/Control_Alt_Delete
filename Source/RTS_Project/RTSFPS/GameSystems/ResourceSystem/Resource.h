// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridClaimingActor.h"


#include "Components/StaticMeshComponent.h"
#include "Resource.generated.h"

USTRUCT()
struct FResourceUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName ResourceName = "";

	UPROPERTY(EditDefaultsOnly)
	UTexture* ResourceImage = nullptr;


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

	void Increment(TSubclassOf<AResource> Key, int Value)
	{
		const int * curval = Find(Key);
		if (curval != nullptr)
		{
			const int newval = *curval + Value;
			Emplace(Key, newval);
		}
		else
		{
			Emplace(Key, Value);
		}
	}

	bool Decrement(TSubclassOf<AResource> Key, int Value)
	{
		const int * curval = Find(Key);
		if (curval != nullptr)
		{
			int newval = *curval - Value;
			if (newval < 0)
			{
				newval = 0;
			}
			Emplace(Key, newval);
			
			return(true);
		}
		else
		{
			Emplace(Key, 0);
			return(false);
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
	TPair<TSubclassOf<AResource>, int > operator[](int Index) const
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
	uint32 GetResourceWeight() const;

	/*Helper Static Function for purchasing Things With Resources*/
	static bool CanAfford(FReplicationResourceMap BuyerResources, FReplicationResourceMap SellerCost);

public:
	/*RTSObject Interface Overrides*/
	virtual void SetSelected() override;
	virtual void SetDeselected() override;
	virtual void RegisterRTSObject() override;
	virtual void UnRegisterRTSObject() override;
	virtual UTexture* GetThumbnail(const UUserWidget* InDisplayContext = nullptr) const override;
	virtual FName GetUnitName() const override;
	/*******************************/

protected:
	virtual void OnResourcesDepleted();
protected:
	/*AActor Override*/
	virtual void BeginPlay() override;

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
	UTexture* Thumbnail = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	FName ResourceName = "Default Resource";
};
