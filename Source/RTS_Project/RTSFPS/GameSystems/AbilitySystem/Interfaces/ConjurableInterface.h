// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "AbilityUserInterface.h"
#include "ConjurableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConjurableInterface : public UInterface
{
	GENERATED_BODY()
};



class RTS_PROJECT_API IConjurableInterface
{
	GENERATED_BODY()

	friend class UConjureAbility;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnConjureDestroyed, TScriptInterface<IConjurableInterface>)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnConjureCompleted, TScriptInterface<IConjurableInterface>)

public:
	virtual void OnConjureStopped();
	virtual void OnConjureStarted();

	virtual IAbilityUserInterface * GetConjurer() const;
	virtual bool IsConjured() const;
	virtual TWeakObjectPtr<UAbility> GetSpawnedAbility() const;
	virtual TSubclassOf<UAbility> GetSpawnAbilityClass() const;

protected:
	virtual void ConjureInit(IAbilityUserInterface* InConjurer, UConjureAbility* InAbility);
	virtual void OnConjureComplete();

public:
	FOnConjureDestroyed OnDestroyedDelegate = FOnConjureDestroyed();
	FOnConjureCompleted OnCompletedDelegate = FOnConjureCompleted();

};
