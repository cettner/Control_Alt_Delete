// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/Properties/RTSActiveProperty.h"

#include "RTSOrder.generated.h"

constexpr int ORDER_APPLY_ALL = -1;

UCLASS(Blueprintable)
class RTS_PROJECT_API URTSOrder : public URTSActiveProperty
{
	GENERATED_BODY()

	public:
		bool RequiresTarget() const;

		int GetApplicationCount() const;

	protected:

		UPROPERTY(EditDefaultsOnly)
		bool bRequiresTarget = false;

		UPROPERTY(EditDefaultsOnly)
		int PropertyApplicationCount = 1;

};
