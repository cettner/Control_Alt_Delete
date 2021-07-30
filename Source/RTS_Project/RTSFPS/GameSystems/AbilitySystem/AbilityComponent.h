// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability.h"
#include "AbilityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

public:
	virtual TWeakObjectPtr<UAbility> GetCurrentAbility();
	virtual bool IsAbilityUsingCrosshair();
	virtual bool IsUsingAbility();
	virtual float GetManaCost();
	virtual bool GetIsCasting();
	virtual bool IsAbilityValid();

protected:
	virtual FVector GetControlRotation();
	

protected:
	virtual FTransform GetSurfaceTransform();


protected:
	UAbility * CurrentAbility = nullptr;
};
