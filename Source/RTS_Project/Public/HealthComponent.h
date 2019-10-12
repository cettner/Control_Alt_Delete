// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "HealthComponent.generated.h"

USTRUCT()
struct FHealthData
{
	GENERATED_USTRUCT_BODY()
		
};

USTRUCT()
struct FDeathData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool ShouldRagdoll;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	virtual void SetMaxHealth(float healthval);

	virtual bool IsAlive();

	virtual float HandleDamageEvent(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser);



	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;

	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, class AController* Killer, class AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnRep_CurrentHealth();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	/////////////////////////////////////////////////
	//Life
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float Current_Health;

	UPROPERTY(EditDefaultsOnly, Category="Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Modifiers")
	TMap<TSubclassOf<UDamageType>, float> Mitigation;

	//////////////////////////////////////////////////
	//Death

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	bool ShouldRagdoll;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float TimeRagdoll;

	/** Animation Played On Pawn Mesh Before Destruction/RagDoll*/
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* DeathAnim;

	/*If death effects are currently playing*/
	bool bIsDying = false;


};
