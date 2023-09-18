// Fill out your copyright notice in the Description page of Project Settings.


#include "ConjurableActorBase.h"
#include "../Abilities/ConjureAbility.h"

#include "Net/UnrealNetwork.h"

// Sets default values
AConjurableActorBase::AConjurableActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = StartConjureOnSpawn;
	bReplicates = true;
}

void AConjurableActorBase::ProcessConjureState()
{
	if (HasAuthority())
	{
		if (CurrentConjureTime >= ConjureTimeToComplete)
		{
			CurrentConjureTime = ConjureTimeToComplete;
			OnConjureComplete();
		}
		else if (CurrentConjureTime <= 0.0f)
		{
			CurrentConjureTime = 0.0f;
			if (OnDestroyedDelegate.IsBound())
			{
				OnDestroyedDelegate.Broadcast(this);
			}

			if (HasAuthority())
			{
				Destroy();
			}
		}
	}
}
void AConjurableActorBase::SetIsConjured(bool InIsConjured)
{
	if (HasAuthority())
	{
		bIsConjured = InIsConjured;
	}
}
void AConjurableActorBase::SetIsConjuring(bool InIsConjuring)
{
	if (HasAuthority())
	{
		bIsConjuring = InIsConjuring;
	}
}
void AConjurableActorBase::ConjureInit(IAbilityUserInterface* InConjurer, UConjureAbility* InAbility)
{
	SpawnedAbilityClass = InAbility->GetClass();
	SpawnedAbilityPtr = TWeakObjectPtr<UConjureAbility>(InAbility);
	PrimaryConjurer = InConjurer;
}

void AConjurableActorBase::OnConjureStarted()
{
	if (!bIsConjured)
	{
		SetIsConjuring(true);
		SetActorTickEnabled(true);
	}
}

void AConjurableActorBase::OnConjureStopped()
{
	SetIsConjured(false);
	SetIsConjuring(false);
	SetActorTickEnabled(true);
}

IAbilityUserInterface* AConjurableActorBase::GetConjurer() const
{
	return PrimaryConjurer;
}

bool AConjurableActorBase::IsConjured() const
{
	return bIsConjured;
}

TWeakObjectPtr<UAbility> AConjurableActorBase::GetSpawnedAbility() const
{
	return SpawnedAbilityPtr;
}

TSubclassOf<UAbility> AConjurableActorBase::GetSpawnAbilityClass() const
{
	return SpawnedAbilityClass;
}

void AConjurableActorBase::OnConjureComplete()
{
	SetIsConjured(true);
	SetActorTickEnabled(false);
	if (OnCompletedDelegate.IsBound())
	{
		OnCompletedDelegate.Broadcast(this);
	}
}

void AConjurableActorBase::Tick(float DeltaSecond)
{
	if (bIsConjuring == true)
	{
		CurrentConjureTime += DeltaSecond;
	}
	else
	{
		CurrentConjureTime -= DeltaSecond;
	}
	ProcessConjureState();
}

void AConjurableActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AConjurableActorBase, bIsConjured);
	DOREPLIFETIME(AConjurableActorBase, bIsConjuring);
}

void AConjurableActorBase::OnRep_IsConjured()
{
	if (!IsConjured())
	{
		SetActorTickEnabled(true);
	}
	else
	{
		OnConjureComplete();
	}
}

void AConjurableActorBase::OnRep_IsConjuring()
{
}
