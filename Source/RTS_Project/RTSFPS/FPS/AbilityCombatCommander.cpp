// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCombatCommander.h"
#include "Net/UnrealNetwork.h"

void AAbilityCombatCommander::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnReadyNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnLoopNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEffectNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnEndNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEndNotify(CallingContext);
	}
}

TArray<TWeakObjectPtr<UAbility>> AAbilityCombatCommander::GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);
	TArray<TWeakObjectPtr<UAbility>> retval = TArray<TWeakObjectPtr<UAbility>>();
	if (abilityweapon != nullptr)
	{
		retval = abilityweapon->GetAbilitiesByClass(AbilityClass);
	}
	return retval;
}

void AAbilityCombatCommander::AddAbility(TSubclassOf<UAbility> InAbilityClass, AActor* InSource)
{
	IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	abilityweapon->AddAbility(InAbilityClass, InSource);
}

void AAbilityCombatCommander::CalculateCurrentWeight()
{
	uint32 calcweight = 0U;
	for (int i = 0; i < HeldResources.Num(); i++)
	{
		const TPair<TSubclassOf<AResource>, int> pair = HeldResources[i];
		const AResource * defaultclass = pair.Key->GetDefaultObject<AResource>();
		const int resourcecount = pair.Value;
		const uint32 resourceweight = defaultclass->GetResourceWeight();
		
		calcweight += (resourceweight * resourcecount);
	}

	CurrentWeight = calcweight;
}

void AAbilityCombatCommander::AddResource(TSubclassOf<AResource> ResourceClass, int amount)
{
	HeldResources.Increment(ResourceClass, amount);
	CalculateCurrentWeight();
}

bool AAbilityCombatCommander::RemoveResource(TSubclassOf<AResource> ResourceClass, int amount)
{
	const bool retval = HeldResources.Decrement(ResourceClass, amount);
	CalculateCurrentWeight();
	return retval;
}

FReplicationResourceMap AAbilityCombatCommander::GetAllHeldResources() const
{
	return HeldResources;
}

uint32 AAbilityCombatCommander::CanCarryMore(TSubclassOf<AResource> ResourceClass) const
{
	uint32 retval = 0U;
	const AResource * defaultresource = ResourceClass->GetDefaultObject<AResource>();
	const uint32 singleresourceweight = defaultresource->GetResourceWeight();
	
	if (singleresourceweight == 0U)
	{
		retval = 0xFFFFFFFFU;
	}
	else
	{
		const uint32 currentweight = GetCurrentWeight();

		if (currentweight < MaxWeight)
		{
			const uint32 availableweight = MaxWeight - currentweight;
			retval = availableweight / singleresourceweight;
		}
	}

	return retval;
}

uint32 AAbilityCombatCommander::GetCurrentWeight() const
{
	return CurrentWeight;
}

uint32 AAbilityCombatCommander::GetMaxWeight() const
{
	return MaxWeight;
}

void AAbilityCombatCommander::GrantExp(uint32 inexp)
{
	const APlayerController * pc =  GetController<APlayerController>();
	IExpAccumulatorInterface* expstate = pc->GetPlayerState<IExpAccumulatorInterface>();
	expstate->GrantExp(inexp);

}

void AAbilityCombatCommander::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAbilityCombatCommander, MaxWeight);
	DOREPLIFETIME(AAbilityCombatCommander, CurrentWeight);
}
