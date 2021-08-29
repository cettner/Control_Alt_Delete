// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../AbilityComponent.h"
#include "AbilityUserInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityUserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IAbilityUserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
		virtual bool CanCastAbility();
		virtual float PlayAbilityMontage(FAbilityAnim AnimToPlay);
		virtual void StopAbilityMontage(FAbilityAnim AnimToStop);
		virtual FVector GetAbilitySocketLocation(FName SocketName);
		virtual FVector GetAbilityAimVector() const;
		virtual TArray<AActor *> GetIgnoredTraceActors(TWeakObjectPtr<UAbility> TracingAbility = nullptr);

		virtual void OnReadyNotify();
		virtual void OnLoopNotify();
		virtual void OnEffectNotify();
		virtual void OnEndNotify();
};
