// Fill out your copyright notice in the Description page of Project Settings.


#include "ConjureAbility.h"
#include "../Interfaces/ConjurableInterface.h"
#include "Kismet/GameplayStatics.h"
#include "../AbilityComponent.h"


FTransform UConjureAbility::DetermineConjureTransform(const int32 InConjureIndex) const
{
	FTransform retval = FTransform();
	IAbilityUserInterface* abilityuser = AbilityComp->GetAbilityUser();
	const FVector aimdirection = abilityuser->GetAbilityAimVector();
	const FVector socketlocation = abilityuser->GetAbilitySocketLocation(EffectSocketName);
	const FVector endtrace = socketlocation + (aimdirection * 85 * (InConjureIndex + 1));

	retval.SetLocation(endtrace);
	retval.SetRotation(aimdirection.ToOrientationQuat());
	return retval;
}

bool UConjureAbility::CanConjureActor() const
{
	return false;
}

void UConjureAbility::ConjureActors()
{
	if (ConjuredActors.Num())
	{
		for (int i = 0; i < ConjuredActors.Num(); i++)
		{
			if (IsValid(ConjuredActors[i].GetObject()))
			{
				ConjuredActors[i]->OnConjureStarted();
			}
			else
			{
				ConjuredActors.RemoveAt(i);
			}
		}
	}
	else if (ConjuredClass != nullptr)
	{
		UWorld* world = GetWorld();
		TArray<TScriptInterface<IConjurableInterface>> preinitconjures = TArray<TScriptInterface<IConjurableInterface>>();
		for (int i = 0; i < ConjureCount; i++)
		{
			const FTransform conjuretransform = DetermineConjureTransform(i);
			AActor* spawnedactor = world->SpawnActorDeferred<AActor>(ConjuredClass, conjuretransform);
			TScriptInterface<IConjurableInterface> conjuredactor = TScriptInterface<IConjurableInterface>(spawnedactor);
			PreInitializeConjuredActors(conjuredactor);
			UGameplayStatics::FinishSpawningActor(spawnedactor, conjuretransform);
			ConjuredActors.Emplace(conjuredactor);
		}
	}
}

void UConjureAbility::DeConjureActors()
{
}

void UConjureAbility::PreInitializeConjuredActors(TScriptInterface<IConjurableInterface>& InConjuredActor)
{
	InConjuredActor->ConjureInit(AbilityComp->GetAbilityUser(), this);
	InConjuredActor->OnCompletedDelegate.AddUFunction(this, "OnConjuredActorCompleted");
	InConjuredActor->OnDestroyedDelegate.AddUFunction(this, "OnConjuredActorDestroyed");
}

void UConjureAbility::OnConjuredActorDestroyed(TScriptInterface<IConjurableInterface> InDestroyingActor)
{
	ConjuredActors.Remove(InDestroyingActor);
}

void UConjureAbility::OnConjuredActorCompleted(TScriptInterface<IConjurableInterface> InDestroyingActor)
{
}

void UConjureAbility::OnEffect()
{
	if(HasAuthority())
	{
		ConjureActors();
	}
}

void UConjureAbility::OnAbilityReleased()
{
	Super::OnAbilityReleased();
	OnAbilityEnd();
}

void UConjureAbility::OnAbilityEnd()
{
	Super::OnAbilityEnd();

	if (HasAuthority())
	{
		for (int32 i = 0; i < ConjuredActors.Num(); i++)
		{
			if (IsValid(ConjuredActors[i].GetObject()))
			{
				ConjuredActors[i]->OnConjureStopped();
			}
			else
			{
				ConjuredActors.RemoveAt(i);
			}
		}
	}
}
