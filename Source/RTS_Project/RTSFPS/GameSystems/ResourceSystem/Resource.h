// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridAttachmentActor.h"

#include "Components/StaticMeshComponent.h"
#include "Resource.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnResourceNodeDestroyedDelegate)

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

	bool Remove(TSubclassOf<AResource> Key)
	{
		bool retval = false;
		int index = Keys.IndexOfByKey(Key);

		if (index != INDEX_NONE)
		{
			Keys.RemoveAt(index);
			Values.RemoveAt(index);
			retval = true;
		}

		return retval;
	}

	bool Increment(TSubclassOf<AResource> Key, int Value)
	{
		bool retval = false;

		if (const int* curval = Find(Key))
		{
			const int newval = *curval + Value;
			Emplace(Key, newval);
			retval = true;
		}

		return retval;
	}

	bool Decrement(TSubclassOf<AResource> Key, int Value)
	{
		bool retval = false;

		if (const int* curval = Find(Key))
		{
			int newval = *curval - Value;
			if (newval < 0)
			{
				newval = 0;
			}
			Emplace(Key, newval);
			retval = true;
		}

		return retval;
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
class RTS_PROJECT_API AResource : public AGridAttachmentActor, public IRTSObjectInterface
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
	virtual int GetTeam() const override;
	/*******************************/

protected:
	virtual void OnResourcesDepleted();
protected:
	/*AActor Override*/
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void PostInitializeComponents() override;

public:
	FOnResourceNodeDestroyedDelegate OnResourceDestroyedDelegate = FOnResourceNodeDestroyedDelegate();

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
