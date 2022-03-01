// Fill out your copyright notice in the Description page of Project Settings.


#include "MageHands.h"
#include "../../CombatCommander.h"

AMageHands::AMageHands() : Super()
{
	bHasSecondary = true;
	if(HasAuthority())
	{
		RightHandAbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("RightHandAbilityComp"));
		check(RightHandAbilityComp);
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

USkeletalMeshComponent * AMageHands::GetWeaponMesh() const
{
	return(MyPawn->GetPawnMesh());
}

bool AMageHands::InitAbilities(IAbilityUserInterface * InAbilUser)
{
	bool retval = Super::InitAbilities(InAbilUser);

	RightHandAbilityComp->InitAbilities(InAbilUser, RightAbilityClasses);

	RightHandAbilityIndex = RightHandAbilityComp->GetCurrentAbilityIndex();
	
	return(retval);
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

void AMageHands::AddResource(TSubclassOf<AResource> ResourceClass, int amount)
{
	Cast<IResourceGatherer>(GetPawnOwner())->AddResource(ResourceClass, amount);
}

uint32 AMageHands::GetCurrentWeight() const
{
	return Cast<IResourceGatherer>(GetPawnOwner())->GetCurrentWeight();
}

uint32 AMageHands::GetMaxWeight() const
{
	return Cast<IResourceGatherer>(GetPawnOwner())->GetMaxWeight();
}

uint32 AMageHands::CanCarryMore(TSubclassOf<AResource> ResourceClass) const
{
	return Cast<IResourceGatherer>(GetPawnOwner())->CanCarryMore(ResourceClass);
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