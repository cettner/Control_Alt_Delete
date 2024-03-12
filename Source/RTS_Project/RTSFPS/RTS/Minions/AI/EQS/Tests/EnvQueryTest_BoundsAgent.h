// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Item.h"
#include "EnvQueryTest_BoundsAgent.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UEnvQueryTest_BoundsAgent : public UEnvQueryTest
{
	GENERATED_UCLASS_BODY()

	protected:
		virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
		virtual FText GetDescriptionTitle() const override;
		virtual FText GetDescriptionDetails() const override;

	protected:
		UPROPERTY(EditDefaultsOnly, Category = Bounds)
		TSubclassOf<UEnvQueryContext> Context = nullptr;
};
