// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability.h"
#include "../Interfaces/ConjurableInterface.h"

#include "ConjureAbility.generated.h"


UCLASS()
class RTS_PROJECT_API UConjureAbility : public UAbility
{
	GENERATED_BODY()

protected:
	virtual FTransform DetermineConjureTransform(const int32 SpawnIndex) const;
	virtual bool CanConjureActor() const;
	virtual void ConjureActors();
	virtual void DeConjureActors();
	virtual void PreInitializeConjuredActors(TScriptInterface<IConjurableInterface>& InConjuredActor);

	UFUNCTION()
	virtual void OnConjuredActorDestroyed(TScriptInterface<IConjurableInterface> InDestroyingActor);
	UFUNCTION()
	virtual void OnConjuredActorCompleted(TScriptInterface<IConjurableInterface> InDestroyingActor);

protected:
	virtual void OnEffect() override;
	virtual void OnAbilityEnd() override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "ConjurableInterface"))
	TSubclassOf<AActor> ConjuredClass = nullptr;

	/*Number of Conjured Actors*/
	UPROPERTY(EditDefaultsOnly)
	int32 ConjureCount = 1;

protected:
	TArray<TScriptInterface<IConjurableInterface>> ConjuredActors = TArray<TScriptInterface<IConjurableInterface>>();
};
