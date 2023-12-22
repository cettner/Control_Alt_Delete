// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/Properties/RTSActiveProperty.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "RTSOrder.generated.h"

constexpr int ORDER_APPLY_ALL = -1; 
#define INVALID_ORDER_ID 0xFFFFFFFF


/*Forward Declaration*/
class IRTSObjectInterface;
class URTSOrderGroup;

UCLASS(Blueprintable)
class RTS_PROJECT_API URTSOrder : public URTSActiveProperty
{
	GENERATED_BODY()

	public:
		FORCEINLINE int GetApplicationCount() const { return PropertyApplicationCount;}

		FORCEINLINE bool UsesExactApplicationCount() const { return bUseExactApplicationCountOnly; }

		/*Whether the order triggers the Default Command for Remaining Selection*/
		FORCEINLINE bool UseDefaultOnFail() const { return bUseDefaultOnFail; }

		virtual void LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const;

		virtual TArray<TScriptInterface<IRTSObjectInterface>> GetBestMinionsForOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InMinionSet, const FHitResult& InTarget = FHitResult()) const;

		virtual URTSOrderGroup* GetOrderGroup() const;

		uint32 GetOrderID() const;

		void SetOrderGroup(URTSOrderGroup* InOrderGroup) { OrderGroup = InOrderGroup; }

	protected:
		/*The number of units that will be exclusively ordered */
		UPROPERTY(EditDefaultsOnly)
		int PropertyApplicationCount = ORDER_APPLY_ALL;

		UPROPERTY(EditDefaultsOnly)
		bool bUseExactApplicationCountOnly = false;

		UPROPERTY(EditDefaultsOnly)
		bool bUseDefaultOnFail = false;

	protected:
		URTSOrderGroup* OrderGroup = nullptr;
};
