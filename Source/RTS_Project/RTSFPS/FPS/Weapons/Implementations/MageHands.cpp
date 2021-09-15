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

void AMageHands::InitAbilities()
{
	Super::InitAbilities();

	for (int i = 0; i < RightAbilityClasses.Num(); i++)
	{
		RightHandAbilityComp->AddAbility(RightAbilityClasses[i]);
	}

}

void AMageHands::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	check(utilizedcomp);
	utilizedcomp->OnReadyNotify();
}


void AMageHands::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	check(utilizedcomp);
	utilizedcomp->OnLoopNotify();
}

void AMageHands::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	check(utilizedcomp);
	utilizedcomp->OnEffectNotify();
}

void AMageHands::OnEndNotify(UAbilityAnimNotify * CallingContext) 
{
	/*Determine if we use the right or left arms component based off the animation*/
	UAbilityComponent * utilizedcomp = GetHandAbilityComponent(CallingContext);
	check(utilizedcomp);
	utilizedcomp->OnEndNotify();
}


void AMageHands::StartSecondaryFire()
{
	ServerStartUseSecondAbility();
}

void AMageHands::StopSecondaryFire()
{
	ServerStopUseSecondAbility();
}

bool AAbilityWeapon::ServerStartUseSecondAbility_Validate()
{
	return true;
}

void AAbilityWeapon::ServerStartUseSecondAbility_Implementation()
{
	RightHandAbilityComp->StartAbility();
}

bool AAbilityWeapon::ServerStopUseSecondAbility_Validate()
{
	return true;
}

void AAbilityWeapon::ServerStopUseSecondAbility_Implementation()
{
	RightHandAbilityComp->ReleaseAbility();
}