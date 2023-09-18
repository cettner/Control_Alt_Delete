// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Interfaces/ConjurableInterface.h"
#include "ConjurableActorBase.generated.h"

UCLASS()
class RTS_PROJECT_API AConjurableActorBase : public AActor, public IConjurableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConjurableActorBase();


protected:
	virtual void ProcessConjureState();
	void SetIsConjured(bool InIsConjured);
	void SetIsConjuring(bool InISConjuring);

public:
	virtual void ConjureInit(IAbilityUserInterface* InConjurer, UConjureAbility* InAbility) override;
	virtual void OnConjureStarted() override;
	virtual void OnConjureStopped() override;

	virtual IAbilityUserInterface* GetConjurer() const override;
	virtual bool IsConjured() const override;
	virtual TWeakObjectPtr<UAbility> GetSpawnedAbility() const override;
	virtual TSubclassOf<UAbility> GetSpawnAbilityClass() const override;

protected:
	virtual void OnConjureComplete() override;

protected:
	virtual void Tick(float DeltaSecond) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	
protected:
	UFUNCTION()
	virtual void OnRep_IsConjured();

	UFUNCTION()
	virtual void OnRep_IsConjuring();

protected:
	UPROPERTY(EditDefaultsOnly)
	float ConjureTimeToComplete = 1.00f;

	UPROPERTY(EditDefaultsOnly)
	bool StartConjureOnSpawn = true;

protected:
	TSubclassOf<UConjureAbility> SpawnedAbilityClass = nullptr;
	TWeakObjectPtr<UAbility> SpawnedAbilityPtr = TWeakObjectPtr<UAbility>();
	IAbilityUserInterface* PrimaryConjurer = nullptr;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsConjured)
	bool bIsConjured = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsConjuring)
	bool bIsConjuring = true;

	float CurrentConjureTime = 0.0f;
};
