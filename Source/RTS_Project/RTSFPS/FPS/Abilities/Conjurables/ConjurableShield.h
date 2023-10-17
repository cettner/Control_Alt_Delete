// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../GameSystems/AbilitySystem/Conjurables/ConjurableActorBase.h"
#include "GameFramework/DamageType.h"
#include "ConjurableShield.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AConjurableShield : public AConjurableActorBase
{
	GENERATED_BODY()
	public:
		AConjurableShield();

	protected:
		virtual void PostInitializeComponents() override;
		virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	protected:
		virtual void ProcessConjureState() override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ShieldMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageConversionType = UDamageType::StaticClass();

	protected:
	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* ShieldMaterial = nullptr;
};
