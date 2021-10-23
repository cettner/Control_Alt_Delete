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

bool AMageHands::InitAbilities()
{
	bool retval = Super::InitAbilities();

	for (int i = 0; i < RightAbilityClasses.Num(); i++)
	{
		RightHandAbilityComp->AddAbility(RightAbilityClasses[i]);
	}

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


void AMageHands::StartSecondaryFire()
{
	ServerStartUseSecondAbility();
}

void AMageHands::StopSecondaryFire()
{
	ServerStopUseSecondAbility();
}

bool AMageHands::ServerStartUseSecondAbility_Validate()
{
	return true;
}

void AMageHands::ServerStartUseSecondAbility_Implementation()
{
	RightHandAbilityComp->StartAbility();
}

bool AMageHands::ServerStopUseSecondAbility_Validate()
{
	return true;
}

void AMageHands::ServerStopUseSecondAbility_Implementation()
{
	RightHandAbilityComp->ReleaseAbility();
}