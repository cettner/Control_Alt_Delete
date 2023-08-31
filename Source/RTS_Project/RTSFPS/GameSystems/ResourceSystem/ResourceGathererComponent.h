// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Interfaces/ResourceGatherer.h"
#include "ResourceGathererComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnResourceDepletedDelegate, TSubclassOf<AResource>, TScriptInterface<IResourceGatherer>)

UCLASS()
class RTS_PROJECT_API UResourceGathererComponent : public UActorComponent, public IResourceGatherer
{
	GENERATED_BODY()

	UResourceGathererComponent();

	public:	
		FOnResourceDepletedDelegate& BindResourceDepletionEvent(TSubclassOf<AResource> InResourceType);

	protected:
		virtual void AddResource(TSubclassOf<AResource> ResourceClass, int amount) override;
		virtual bool RemoveResource(const TSubclassOf<AResource> ResourceClass, int amount) override;

	protected:
		virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	protected:
		UPROPERTY(EditDefaultsOnly, Replicated)
		FReplicationResourceMap Resources = FReplicationResourceMap();

	protected:
		TMap<TSubclassOf<AResource>,FOnResourceDepletedDelegate> ResourceDelegates;
};
