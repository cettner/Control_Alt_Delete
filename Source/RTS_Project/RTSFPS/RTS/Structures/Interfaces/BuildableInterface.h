// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BuildableInterface.generated.h"


constexpr float CONSTRUCTION_COMPLETE_THRESHOLD = 1.0f;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IBuildableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetPercentConstructed()const;
	virtual bool IsConstructed() const;
	virtual void BeginConstruction();
	virtual void IncrementConstruction(float DeltaConstruction, AActor* Contributer = nullptr);

protected:
	virtual void OnConstructionComplete();

protected:
	bool bISConstructed = false;

	float PercentConstructed = 0.0f;
};
