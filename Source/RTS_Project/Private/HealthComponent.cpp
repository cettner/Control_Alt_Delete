// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "UnrealNetwork.h"
#include "Engine.h"

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
	if (Damage > 0.0f)
	{
		Current_Health -= Damage;
		if (Current_Health <= 0.0f)
		{
			Die(Damage, DamageEvent, EventInstigator, DamageCauser);
		}
		
		// Make Some Noise if the owner happens to be a Pawn
		if (Cast<APawn>(CompOwner))
		{
			APawn * NoiseMaker = Cast<APawn>(CompOwner);
			float NoiseRange = 300.0f;
			CompOwner->MakeNoise(1.0f, EventInstigator ? EventInstigator->GetPawn() : NoiseMaker);
		}

	}

	return (Current_Health);
}

bool UHealthComponent::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	if (bIsDying 
		|| (CompOwner && CompOwner->IsPendingKill()) 
		|| (CompOwner && CompOwner->Role != ROLE_Authority))
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
	Current_Health = FMath::Min(0.0f, Current_Health);
	return (true);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CompOwner = GetOwner();
	if (!CompOwner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("HealthComponent Initialization Failed!")));
	}
}

void UHealthComponent::OnRep_CurrentHealth()
{
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UHealthComponent,Current_Health)
}


