// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "UnrealNetwork.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealthComponent::SetMaxHealth(float healthval)
{
	MaxHealth = healthval;
}

bool UHealthComponent::IsAlive()
{
	return (Current_Health > 0.0F);
}

float UHealthComponent::HandleDamageEvent(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (Current_Health <= 0.f)
	{
		return 0.f;
	}
	Current_Health -= Damage;

	return (Current_Health);
}

bool UHealthComponent::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	AActor * Owner = GetOwner();
	if (bIsDying 
		|| (Owner && Owner->IsPendingKill()) 
		|| (Owner && Owner->Role != ROLE_Authority))
	{
		return false;
	}
	else
	{
		return true;
	}
	
}

bool UHealthComponent::Die(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser)
{
	return false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::OnRep_CurrentHealth()
{
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UHealthComponent,Current_Health)
}


