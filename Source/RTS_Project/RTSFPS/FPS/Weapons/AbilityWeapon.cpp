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
	if (AbilityIndex > NO_ABILITY_INDEX)
	{
		ServerStartUseAbility(AbilityIndex);
	}
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

	IAbilityUserInterface* abilityuser = Cast<IAbilityUserInterface>(NewOwner);
	checkf(abilityuser,TEXT("AAbilityWeapon::OnEnterInventory : Weapon User Must Inherit IAbilityUserInterface"))
	
	if (bAreAbilitiesInitialized == false)
	{
		bAreAbilitiesInitialized = InitAbilities(abilityuser);
	}

}

void AAbilityWeapon::WeaponPrimarySetting(int ModeToggle)
{
	AbilityIndex = AbilityComp->GetNextAvailableIndex(AbilityIndex);
}

bool AAbilityWeapon::CanCastAbility() const
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

TArray<TWeakObjectPtr<UAbility>> AAbilityWeapon::GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const
{
	return AbilityComp->GetAbilitiesByClass(AbilityClass);
}

void AAbilityWeapon::AddAbility(TSubclassOf<UAbility> InAbilityClass, AActor* InSource)
{
	AbilityComp->SetAbilityEnabledState(InAbilityClass, true);
}

void AAbilityWeapon::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	SetWeaponState(EWeaponState::Firing);
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
	SetWeaponState(EWeaponState::Idle);
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

bool AAbilityWeapon::InitAbilities(IAbilityUserInterface * InUser)
{
	AbilityComp->InitAbilities(this, AbilityClasses);
	AbilityIndex = AbilityComp->GetCurrentAbilityIndex();
	const bool retval = AbilityIndex > NO_ABILITY_INDEX;
	return (retval);
}

void AAbilityWeapon::EndPlay(EEndPlayReason::Type InReason)
{
	AbilityComp->InterruptAbility();
	Super::EndPlay(InReason);
}

bool AAbilityWeapon::ServerStartUseAbility_Validate(int InAbilityIndex)
{
	return true;
}

void AAbilityWeapon::ServerStartUseAbility_Implementation(int InAbilityIndex)
{
	AbilityComp->StartAbility(InAbilityIndex);
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