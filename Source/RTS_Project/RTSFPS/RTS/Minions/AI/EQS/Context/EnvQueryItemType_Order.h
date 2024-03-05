// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "RTS_Project/RTSFPS/RTS/Orders/RTSOrder.h"

#include "CoreMinimal.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvQueryItemType_Order.generated.h"

struct FEnvQueryContextData;
struct FWeakObjectPtr;

UCLASS()
class RTS_PROJECT_API UEnvQueryItemType_Order : public UEnvQueryItemType_VectorBase
{
	GENERATED_BODY()

public:
	static void SetContextHelper(FEnvQueryContextData& ContextData, const URTSOrder* SingleOrder);

	static URTSOrder* GetValue(const uint8* RawData);
	static void SetValue(uint8* RawData, const FWeakObjectPtr& Value);

	virtual FVector GetItemLocation(const uint8* RawData) const override;
	virtual URTSOrder* GetOrder(const uint8* RawData) const;
};
