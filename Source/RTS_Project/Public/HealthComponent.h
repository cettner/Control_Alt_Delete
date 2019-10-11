// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void SetMaxHealth(float healthval);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnRep_CurrentHealth();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float Current_Health;

	UPROPERTY(EditDefaultsOnly, Category="Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Modifiers")
	TMap<TSubclassOf<UDamageType>, float> Mitigation;
};
