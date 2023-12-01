// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Net/UnrealNetwork.h"
#include "Interfaces/ResourceGatherer.h"
#include "ResourceData.h"
#include "ResourceGathererComponent.generated.h"






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
		virtual bool SetResource(const TSubclassOf<UResource>& InResource, const uint32 InAmount) override;

		virtual FReplicationResourceMap GetAllHeldResources() const override;
		virtual FReplicationResourceMap GetAllWeightedResources() const override;
		virtual FReplicationResourceMap GetAllDiscreteResources() const override;

		virtual uint32 GetCurrentWeight() const override;
		virtual uint32 GetMaxWeight() const override;

		virtual void SetResourceDiscreteMaximum(const TSubclassOf<UResource> InResourceClass, uint32 InAmount, const EResourceBoundsAdjustment AdjustmentRules = EResourceBoundsAdjustment::ADJUST_IF_OUT_OF_BOUNDS) override;
		virtual void SetResourceDiscreteMinimum(const TSubclassOf<UResource> InResourceClass, uint32 InAmount, const EResourceBoundsAdjustment AdjustmentRules = EResourceBoundsAdjustment::ADJUST_IF_OUT_OF_BOUNDS) override;

		virtual void AddResourceRegenEvent(FResourceRegenEventConfig InResourceConfig, const TSubclassOf<UResource>& InResourceClass) override;
		virtual bool ClearResourceRegenEvent(const TSubclassOf<UResource>& InResourceClass) override;
		virtual uint32 ClearAllResourceRegenEvents() override;
		virtual const FResourceRegenEventConfig* GetCurrentRegenEventConfig(const TSubclassOf<UResource>& InResourceClass) const override;

	protected:
		virtual uint32 GetResourceDiscreteMaximum(const TSubclassOf<UResource> ResourceClass) const override;
		virtual uint32 GetResourceDiscreteMinimum(const TSubclassOf<UResource> ResourceClass) const override;

		virtual void SetMaxWeight(const uint32 InAmount, const EWeightedResourceBoundsAdjustment AdjustmentRules = EWeightedResourceBoundsAdjustment::REMOVE_SMALLEST) override;

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
		void HandleResourceRegenEvent(TSubclassOf<UResource> InEventKey);

	protected:
		UFUNCTION()
		virtual void OnRep_HeldResources();

	protected:
		virtual void OnRegister() override;
		virtual void BeginPlay() override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	protected:
		UPROPERTY(EditDefaultsOnly)
		UResourceData* StartingResources = nullptr;

	protected:
		UPROPERTY(Replicated)
		uint32 MaxWeight = 10U;

		UPROPERTY(ReplicatedUsing = OnRep_HeldResources)
		TArray<uint32> Values;

		// Map that holds the Keys and the index the Value is at in the parallel array
		TMap<TSubclassOf<UResource>, int> ResourceToIndex = TMap<TSubclassOf<UResource>, int>();
		
		UPROPERTY(Replicated)
		FReplicationResourceMap ResourceMaximums = FReplicationResourceMap();

		TMap<TSubclassOf<UResource>, FResourceRegenEventConfig> ActiveRegenEvents = TMap<TSubclassOf<UResource>, FResourceRegenEventConfig>();

		TMap<TSubclassOf<UResource>, FOnResourceValueChangedDelegate> ResourceDelegates = TMap<TSubclassOf<UResource>, FOnResourceValueChangedDelegate>();

		uint32 CurrentWeight = 0U;
};
