// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSOrder.h"
#include "../../GameSystems/ResourceSystem/Resource.h"

#include "RTSResourcePurchaseOrder.generated.h"



UCLASS(Blueprintable)
class RTS_PROJECT_API URTSResourcePurchaseOrder : public URTSOrder
{
	GENERATED_BODY()

	public:
		void SetPurchaseClass(const TSubclassOf<UObject> InClass);
		void SetPrice(const FReplicationResourceMap InPriceMap);

		TSubclassOf<UObject> GetPurchaseClass() const;
		FReplicationResourceMap GetPurchasePrice() const;

	protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	protected:
		UPROPERTY(Replicated)
		TSubclassOf<UObject> PurchaseClass = nullptr;

		UPROPERTY(Replicated)
		FReplicationResourceMap PurchasePrice = FReplicationResourceMap();
};
