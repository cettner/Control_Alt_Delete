// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWeapon.h"
#include "../CombatCommander.h"
#include "Net/UnrealNetwork.h"

AAbilityWeapon::AAbilityWeapon() : Super()
{
	AbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComp"));
	check(AbilityComp);
}

void AAbilityWeapon::StartFire()
{
	ServerStartUseAbility();
}

void AAbilityWeapon::StopFire()
{
	ServerStopUseAbility();
}

void AAbilityWeapon::StartReload(bool bFromReplication)
{
}

void AAbilityWeapon::StopReload()
{
}

bool AAbilityWeapon::CanCastAbility()
{
	return true;
}

float AAbilityWeapon::PlayAbilityMontage(FAbilityAnim AnimToPlay)
{
	FWeaponAnim converter;
	converter.AnimFirstPerson = AnimToPlay.AnimFirstPerson;
	converter.AnimThirdPerson = AnimToPlay.AnimThirdPerson;

	return PlayWeaponAnimation(converter);
}

FVector AAbilityWeapon::GetAbilitySocketLocation(FName SocketName)
{
	FVector retval = GetActorLocation();
	if (GetWeaponMesh() && GetWeaponMesh()->DoesSocketExist(SocketName))
	{
		retval = GetWeaponMesh()->GetSocketLocation(SocketName);
	}

	return retval;
}

FVector AAbilityWeapon::GetAbilityAimVector() const
{
	FVector adjustedaim = GetAdjustedAim();
	FTransform retval;
	APlayerController* PC = MyPawn ? Cast<APlayerController>(MyPawn->Controller) : NULL;
	
	UWorld* world = GetWorld();
	check(world);

	FVector StartTrace = this->GetActorLocation();
	if (PC)
	{
		// use player's camera
		FRotator UnusedRot;
		PC->GetPlayerViewPoint(StartTrace, UnusedRot);
		
		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start
		StartTrace = StartTrace + adjustedaim * ((GetInstigator()->GetActorLocation() - StartTrace) | adjustedaim);
	}
	
	return(StartTrace);
}

int AAbilityWeapon::GetCurrentMana() const
{
	return Mana;
}

void AAbilityWeapon::InitAbilities()
{
	for (int i = 0; i < AbilityClasses.Num(); i++)
	{
		AbilityComp->AddAbility(AbilityClasses[i]);
	}
}

void AAbilityWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	InitAbilities();
}


bool AAbilityWeapon::ServerStartUseAbility_Validate()
{
	return true;
}

void AAbilityWeapon::ServerStartUseAbility_Implementation()
{
	AbilityComp->StartAbility();
}

bool AAbilityWeapon::ServerStopUseAbility_Validate()
{
	return true;
}

void AAbilityWeapon::ServerStopUseAbility_Implementation()
{
	AbilityComp->ReleaseAbility();
}

void AAbilityWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAbilityWeapon, Mana);

}