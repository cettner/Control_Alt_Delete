// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ResourceVendorData.h"
#include "Interfaces/ResourceVendorInterface.h"
#include "ResourceVendorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API UResourceVendorComponent : public UActorComponent, public IResourceVendorInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UResourceVendorComponent();


protected:
	/*********************************Resource Vendor***********************************/
	virtual TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetAllDefaultUnitPrices() const override;
	virtual FReplicationResourceMap GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, const AController* InstigatingController = nullptr) const override;
	/***********************************************************************************/

protected:
	virtual void OnRegister() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/******Config*****/
protected:
	UPROPERTY(EditDefaultsOnly)
	UResourceVendorData* StartingPrices = nullptr;

	/****Runtime*****/
protected:
	TMap<TSubclassOf<UObject>, int32> PriceIndexMap = TMap<TSubclassOf<UObject>, int32>();

	UPROPERTY(Replicated)
	TArray<FReplicationResourceMap> ReplicatedPrices = TArray<FReplicationResourceMap>();
};
