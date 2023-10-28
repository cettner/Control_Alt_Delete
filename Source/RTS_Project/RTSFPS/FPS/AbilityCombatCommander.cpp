// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCombatCommander.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

AAbilityCombatCommander::AAbilityCombatCommander() : Super()
{
	ResourceVendorComp = CreateDefaultSubobject<UResourceVendorComponent>(TEXT("ResourceVendorComp"));
}

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

bool AAbilityCombatCommander::CanCastAbility(const TWeakObjectPtr<UAbility> TracingAbility) const
{
	FReplicationResourceMap abilitycost = TracingAbility->GetAbilityCost();

	bool retval = false;
	
	if (HasResource(abilitycost))
	{
		retval = true;
	}

	return retval;
}

bool AAbilityCombatCommander::SpendAbilityCost(const TWeakObjectPtr<UAbility> SpendingAbility)
{
	FReplicationResourceMap abilitycost = FReplicationResourceMap();
	checkf(GetUnitPriceForSource(SpendingAbility->GetClass(), this, abilitycost, GetController()), TEXT("AAbilityCombatCommander::CanCastAbility Failed to obtain pricemap"));
	bool retval = false;

	if (HasResource(abilitycost))
	{
		retval = IResourceGatherer::RemoveResource(abilitycost);
	}

	return retval;
}

const TMap<TSubclassOf<UObject>, FReplicationResourceMap> AAbilityCombatCommander::GetAllDefaultUnitPrices() const
{
	return ResourceVendorComp->GetAllDefaultUnitPrices();
}

bool AAbilityCombatCommander::GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, FReplicationResourceMap& OutPrices, const AController* InstigatingController) const
{
	return ResourceVendorComp->GetUnitPriceForSource(PurchaseClass,Purchaser, OutPrices, InstigatingController);
}

void AAbilityCombatCommander::GrantExp(uint32 inexp)
{
	const APlayerController * pc =  GetController<APlayerController>();
	APlayerState* ps = pc->GetPlayerState<APlayerState>();
	IExpAccumulatorInterface* expstate = Cast<IExpAccumulatorInterface>(ps);
	expstate->GrantExp(inexp);

}