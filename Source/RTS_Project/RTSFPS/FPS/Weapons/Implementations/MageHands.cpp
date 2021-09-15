// Fill out your copyright notice in the Description page of Project Settings.


#include "MageHands.h"
#include "../../CombatCommander.h"

AMageHands::AMageHands() : Super()
{
	bHasSecondary = true;

	RightHandAbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("RightHandAbilityComp"));
	check(RightHandAbilityComp);
	RightHandAbilityComp->SetIsReplicated(true);
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


void AMageHands::StartSecondaryFire()
{
}

void AMageHands::StopSecondaryFire()
{

}