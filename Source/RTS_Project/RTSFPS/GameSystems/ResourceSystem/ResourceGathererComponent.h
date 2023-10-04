// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Interfaces/ResourceGatherer.h"
#include "ResourceData.h"
#include "ResourceGathererComponent.generated.h"



DECLARE_MULTICAST_DELEGATE_FourParams(FOnResourceValueChangedDelegate, const TSubclassOf<UResource>, const uint32/*old value*/, const uint32 /*new value*/, TScriptInterface<IResourceGatherer>);

UCLASS()
class RTS_PROJECT_API UResourceGathererComponent : public UActorComponent, public IResourceGatherer
{
	GENERATED_BODY()

	UResourceGathererComponent();

	public:	
		FOnResourceValueChangedDelegate& BindResourceValueChangedEvent(const TSubclassOf<UResource> InResourceType);

	public:
		virtual void AddResource(TSubclassOf<UResource> ResourceClass, int amount) override;
		virtual bool RemoveResource(const TSubclassOf<UResource> ResourceClass, int amount) override;
		virtual FReplicationResourceMap GetAllHeldResources() const override;
		virtual uint32 GetCurrentWeight() const override;
		virtual uint32 GetMaxWeight() const override;

		virtual void SetResourceDiscreteMaximum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount) override;
		virtual void SetResourceDiscreteMinimum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount) override;

	protected:
		virtual uint32 GetResourceDiscreteMaximum(const TSubclassOf<UResource> ResourceClass) const override;
		virtual uint32 GetResourceDiscreteMinimum(const TSubclassOf<UResource> ResourceClass) const override;

	protected:
		virtual void RecalculateWeight();

	// Anthonys cool code club ------------------------------------------------------
	private:
		// Adds new key value pair to the back
		void Emplace(const TSubclassOf<UResource> InKey, const uint32 InValue);

		bool Remove(TSubclassOf<UResource> Key);

		bool IncOrDec(TSubclassOf<UResource> Key, uint32 Value, bool Increment=true);

		bool Increment(TSubclassOf<UResource> Key, uint32 Value);

		bool Decrement(TSubclassOf<UResource> Key, uint32 Value);

		const uint32* Find(TSubclassOf<UResource> Key) const;

		const int Num() const;
																								   
	// END ---
	protected:
		UFUNCTION()
		virtual void OnRep_HeldResources();

	protected:
		virtual void OnRegister() override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	protected:
		UPROPERTY(EditDefaultsOnly)
		UResourceData* StartingResources = nullptr;

	protected:
		UPROPERTY(Replicated)
		uint32 MaxWeight = 10U;

		// TODO: Remove
		//UPROPERTY(ReplicatedUsing = OnRep_HeldResources)
		//FReplicationResourceMap HeldResources = FReplicationResourceMap();

		 // New code 
		UPROPERTY(ReplicatedUsing = OnRep_HeldResources)
		TArray<uint32> Values;
		
		// Will be set at start of game
		//TArray<TSubclassOf<UResource>> Keys;

		// Map that holds the Keys and the index the Value is at in the parallel array
		TMap<TSubclassOf<UResource>, int> ResourceToIndex;


		
		

		UPROPERTY(Replicated)
		FReplicationResourceMap ResourceMaximums = FReplicationResourceMap();

		TMap<TSubclassOf<UResource>, FOnResourceValueChangedDelegate> ResourceDelegates;

		uint32 CurrentWeight = 0U;
};
