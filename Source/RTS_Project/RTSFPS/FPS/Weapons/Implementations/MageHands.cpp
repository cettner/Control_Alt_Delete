// Fill out your copyright notice in the Description page of Project Settings.


#include "MageHands.h"
#include "../../CombatCommander.h"

AMageHands::AMageHands() : Super()
{
	bHasSecondary = true;
	if(HasAuthority())
	{
		RightHandAbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("RightHandAbilityComp"));
		RightHandAbilityComp->SetIsReplicated(true);
	}

}

UAbilityComponent * AMageHands::GetHandAbilityComponent(UAbilityAnimNotify * CallingContext) const
{	
	UAbilityComponent * retval = nullptr;

	FName slotname = "";
	if(CallingContext != nullptr)
	{
		slotname = CallingContext->GetAssetSlotName();
	}

	if(slotname == "LeftArm")
	{
		 retval = AbilityComp;
	}
	else if(slotname == "RightArm") 
	{
		retval = RightHandAbilityComp;
	}

	return retval;
}

void AMageHands::OnRightHandAbilityEnableStateChanged(TWeakObjectPtr<UAbility> SpawningAbility)
{
	if (RightHandAbilityIndex == NO_ABILITY_INDEX)
	{
		RightHandAbilityIndex = RightHandAbilityComp->GetNextAvailableIndex();
	}
}

void AMageHands::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	RightHandAbilityComp->AbilityChangeDelegate.AddDynamic(this, &AMageHands::OnRightHandAbilityEnableStateChanged);
}

USkeletalMeshComponent * AMageHands::GetWeaponMesh() const
{
	return(MyPawn->GetPawnMesh());
}

bool AMageHands::InitAbilities(IAbilityUserInterface * InAbilUser)
{
	bool retval = Super::InitAbilities(InAbilUser);

	RightHandAbilityComp->InitAbilities(this, RightAbilityClasses);

	RightHandAbilityIndex = RightHandAbilityComp->GetCurrentAbilityIndex();
	
	return(retval);
}

void AMageHands::OnResourceSourceChanged(const TSubclassOf<UResource> InClass, const uint32 InOldValue, const uint32 InNewValue, TScriptInterface<IResourceGatherer> InSource)
{
	Super::OnResourceSourceChanged(InClass, InOldValue, InNewValue, InSource);

	if (RightHandAbilityComp->IsCasting() && RightHandAbilityComp->WantstoCast())
	{
		const UAbility* currentability = RightHandAbilityComp->GetCurrentAbility();
		FReplicationResourceMap abilitycost = currentability->GetAbilityCost();
		if (!InSource->HasResource(abilitycost))
		{
			RightHandAbilityComp->InterruptAbility();
		}
	}
}

void AMageHands::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	SetWeaponState(EWeaponState::Firing);
	utilizedcomp->OnReadyNotify();
}

void AMageHands::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	utilizedcomp->OnLoopNotify();

}

void AMageHands::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	utilizedcomp->OnEffectNotify();
}

void AMageHands::OnEndNotify(UAbilityAnimNotify * CallingContext) 
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	utilizedcomp->OnEndNotify();
	if (!AbilityComp->IsCastReady() && !RightHandAbilityComp->IsCastReady())
	{
		SetWeaponState(EWeaponState::Idle);
	}

}

TArray<TWeakObjectPtr<UAbility>> AMageHands::GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const
{
	TArray<TWeakObjectPtr<UAbility>> retval = Super::GetAbilitiesByClass(AbilityClass);
	retval.Append(RightHandAbilityComp->GetAbilitiesByClass(AbilityClass));
	return retval;
}

void AMageHands::AddAbility(TSubclassOf<UAbility> InAbilityClass, AActor* InSource)
{
	const UAbility* defaultability = InAbilityClass.GetDefaultObject();
	const TArray<FName> abilitytags = defaultability->GetAbilityTags();
	const bool ismainhandability = abilitytags.Contains(MainHandAbilityTag);
	const bool bisoffhandability = abilitytags.Contains(OffHandAbilityTag);
	

	if (bisoffhandability == true && ismainhandability == true)
	{
		RightHandAbilityComp->SetAbilityEnabledState(InAbilityClass, true);
		AbilityComp->SetAbilityEnabledState(InAbilityClass, true);
	}
	else if (bisoffhandability == true)
	{
		RightHandAbilityComp->SetAbilityEnabledState(InAbilityClass, true);
	}
	else
	{
		AbilityComp->SetAbilityEnabledState(InAbilityClass, true);
	}

	/*If we didnt have an ability before and added one get a new index*/
	if (RightHandAbilityIndex == NO_ABILITY_INDEX)
	{
		RightHandAbilityIndex = RightHandAbilityComp->GetCurrentAbilityIndex();
	}

	if (AbilityIndex == NO_ABILITY_INDEX)
	{
		AbilityIndex = AbilityComp->GetCurrentAbilityIndex();
	}
}

void AMageHands::AddResource(TSubclassOf<UResource> ResourceClass, int amount)
{
	Cast<IResourceGatherer>(GetPawnOwner())->AddResource(ResourceClass, amount);
}

bool AMageHands::RemoveResource(TSubclassOf<UResource> ResourceClass, int amount)
{
	return Cast<IResourceGatherer>(GetPawnOwner())->RemoveResource(ResourceClass, amount);
}

uint32 AMageHands::GetCurrentWeight() const
{
	return Cast<IResourceGatherer>(GetPawnOwner())->GetCurrentWeight();
}

uint32 AMageHands::GetMaxWeight() const
{
	return Cast<IResourceGatherer>(GetPawnOwner())->GetMaxWeight();
}

FReplicationResourceMap AMageHands::GetAllHeldResources() const
{
	return Cast<IResourceGatherer>(GetPawnOwner())->GetAllHeldResources();
}

void AMageHands::StartSecondaryFire()
{
	if (RightHandAbilityIndex > NO_ABILITY_INDEX)
	{
		ServerStartUseSecondAbility(RightHandAbilityIndex);
	}
}

void AMageHands::StopSecondaryFire()
{
	ServerStopUseSecondAbility();
}

bool AMageHands::ServerStartUseSecondAbility_Validate(int RequestedAbilityIndex)
{
	return true;
}

void AMageHands::ServerStartUseSecondAbility_Implementation(int InAbilityIndex)
{
	RightHandAbilityComp->StartAbility(InAbilityIndex);
}

bool AMageHands::ServerStopUseSecondAbility_Validate()
{
	return true;
}

void AMageHands::ServerStopUseSecondAbility_Implementation()
{
	RightHandAbilityComp->ReleaseAbility();
}