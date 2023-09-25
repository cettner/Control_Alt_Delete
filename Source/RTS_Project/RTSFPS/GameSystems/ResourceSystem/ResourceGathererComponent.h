// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Interfaces/ResourceGatherer.h"
#include "ResourceData.h"
#include "ResourceGathererComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnResourceDepletedDelegate, TSubclassOf<UResource>, TScriptInterface<IResourceGatherer>)

UCLASS()
class RTS_PROJECT_API UResourceGathererComponent : public UActorComponent, public IResourceGatherer
{
	GENERATED_BODY()

	UResourceGathererComponent();

	public:	
		FOnResourceDepletedDelegate& BindResourceDepletionEvent(TSubclassOf<UResource> InResourceType);

	public:
		virtual void AddResource(TSubclassOf<UResource> ResourceClass, int amount) override;
		virtual bool RemoveResource(const TSubclassOf<UResource> ResourceClass, int amount) override;
		virtual FReplicationResourceMap GetAllHeldResources() const override;
		virtual uint32 GetCurrentWeight() const override;
		virtual uint32 GetMaxWeight() const override;

	protected:
		virtual uint32 GetResourceDiscreteMaximum(const TSubclassOf<UResource> ResourceClass) const override;
		virtual uint32 GetResourceDiscreteMinimum(const TSubclassOf<UResource> ResourceClass) const override;

	protected:
		virtual void RecalculateWeight();

	protected:
		UFUNCTION()
		virtual void OnRep_HeldResources();

	protected:
		virtual void OnRegister() override;
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	protected:
		UPROPERTY(EditDefaultsOnly)
		UResourceData* StartingResources = nullptr;

		UPROPERTY(EditDefaultsOnly)
		uint32 MaxWeight = 10U;

	protected:
		UPROPERTY(ReplicatedUsing = OnRep_HeldResources)
		FReplicationResourceMap HeldResources = FReplicationResourceMap();
		UPROPERTY(Replicated)
		FReplicationResourceMap ResourceMaximums = FReplicationResourceMap();

		TMap<TSubclassOf<UResource>,FOnResourceDepletedDelegate> ResourceDelegates;

		uint32 CurrentWeight = 0U;
};
