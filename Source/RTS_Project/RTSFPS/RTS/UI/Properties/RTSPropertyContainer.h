// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTSProperty.h"

#include "RTSPropertyContainer.generated.h"



UCLASS(Blueprintable)
class RTS_PROJECT_API URTSPropertyContainer : public URTSProperty
{
	GENERATED_BODY()

	public:
		const TArray<TSubclassOf<URTSProperty>> GetNestedProperties()  const;

	protected:
		UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<URTSProperty>> NestedProperties = TArray<TSubclassOf<URTSProperty>>();
};
