// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWeapon.h"
#include "../CombatCommander.h"
#include "Net/UnrealNetwork.h"

AAbilityWeapon::AAbilityWeapon() : Super()
{
	if (HasAuthority())
	{
		AbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComp"));
		check(AbilityComp);
		AbilityComp->SetIsReplicated(true);
	}
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

void AAbilityWeapon::OnEnterInventory(ACombatCommander * NewOwner)
{
	Super::OnEnterInventory(NewOwner);
	if (bAreAbilitiesInitialized == false)
	{
		bAreAbilitiesInitialized = InitAbilities();
	}

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

void AAbilityWeapon::StopAbilityMontage(FAbilityAnim AnimToStop)
{
	FWeaponAnim converter;
	converter.AnimFirstPerson = AnimToStop.AnimFirstPerson;
	converter.AnimThirdPerson = AnimToStop.AnimThirdPerson;

	StopWeaponAnimation(converter);
}

FVector AAbilityWeapon::GetAbilitySocketLocation(FName SocketName) const 
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
	FVector retval = GetAdjustedAim();
	
	return(retval);
}

TArray<AActor*> AAbilityWeapon::GetIgnoredTraceActors(TWeakObjectPtr<UAbility> TracingAbility)
{
	TArray<AActor*> outvec = TArray<AActor*>();
	outvec.Emplace(MyPawn);
	outvec.Emplace(this);
	return outvec;
}

void AAbilityWeapon::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	AbilityComp->OnReadyNotify();
}

void AAbilityWeapon::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
	AbilityComp->OnLoopNotify();
}

void AAbilityWeapon::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
	AbilityComp->OnEffectNotify();
}

void AAbilityWeapon::OnEndNotify(UAbilityAnimNotify * CallingContext)
{
	AbilityComp->OnEndNotify();
}

USceneComponent * AAbilityWeapon::GetParticleAttatchmentComponent(TWeakObjectPtr<UAbility> SpawningAbility)
{
	return GetWeaponMesh();
}

int AAbilityWeapon::GetCurrentMana() const
{
	return Mana;
}

bool AAbilityWeapon::InitAbilities()
{
	for (int i = 0; i < AbilityClasses.Num(); i++)
	{
		AbilityComp->AddAbility(AbilityClasses[i]);
	}

	return (true);
}

void AAbilityWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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