// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWeapon.h"
#include "../CombatCommander.h"
#include "Net/UnrealNetwork.h"

AAbilityWeapon::AAbilityWeapon() : Super()
{
	if (HasAuthority())
	{
		AbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComp"));
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
	
	if (bAreAbilitiesInitialized == false)
	{
		bAreAbilitiesInitialized = InitAbilities(this);
		InitResourceBindings(NewOwner);
	}
}

void AAbilityWeapon::WeaponPrimarySetting(int ModeToggle)
{
	AbilityIndex = AbilityComp->GetNextAvailableIndex(AbilityIndex);
}

bool AAbilityWeapon::CanCastAbility(const UAbility * InAbility) const
{
	IAbilityUserInterface * weaponwielder = CastChecked<IAbilityUserInterface>(GetPawnOwner());
	const bool retval = weaponwielder->CanCastAbility(InAbility);
	return retval;
} 

bool AAbilityWeapon::SpendAbilityCost(const UAbility * SpendingAbility)
{
	IAbilityUserInterface* abilityuser = CastChecked<IAbilityUserInterface>(GetPawnOwner());
	const bool retval = abilityuser->SpendAbilityCost(SpendingAbility);
	return retval;
}

float AAbilityWeapon::PlayAbilityMontage(const FAbilityAnim& AnimToPlay)
{
	FWeaponAnim converter;
	converter.AnimFirstPerson = AnimToPlay.AnimFirstPerson;
	converter.AnimThirdPerson = AnimToPlay.AnimThirdPerson;

	return PlayWeaponAnimation(converter); 
}

void AAbilityWeapon::StopAbilityMontage(const FAbilityAnim& AnimToStop)
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

TArray<UAbility*> AAbilityWeapon::GetAbilitiesByClass(const TSubclassOf<UAbility>& AbilityClass) const
{
	return AbilityComp->GetAbilitiesByClass(AbilityClass);
}

void AAbilityWeapon::EnableAbility(const TSubclassOf<UAbility>& AbilityClass)
{
	if (AbilityComp->SupportsAbility(AbilityClass))
	{
		AbilityComp->EnableAbility(AbilityClass);
	}
}

bool AAbilityWeapon::DisableAbility(const TSubclassOf<UAbility>& AbilityClass)
{
	bool retval = false;
	if (AbilityComp->SupportsAbility(AbilityClass))
	{
		retval = AbilityComp->DisableAbility(AbilityClass);
	}
	return retval;
}

TSet<TSubclassOf<UAbility>> AAbilityWeapon::GetSupportedAbilities() const
{
	return AbilityComp->GetSupportedAbilities();
}

UAbility * AAbilityWeapon::GetFirstAbilityByClass(const TSubclassOf<UAbility>& InAbilityClass) const
{
	return AbilityComp->GetFirstAbilityByClass(InAbilityClass);
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

void AAbilityWeapon::OnTickNotify(float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference)
{
	AbilityComp->OnTickNotify(InFrameDeltaTime, InEventReference);
}

USceneComponent * AAbilityWeapon::GetParticleAttatchmentComponent(TWeakObjectPtr<UAbility> SpawningAbility)
{
	return GetWeaponMesh();
}

bool AAbilityWeapon::InitAbilities(IAbilityUserInterface * InUser)
{
	AbilityComp->InitAbilities(InUser);
	AbilityIndex = AbilityComp->GetCurrentAbilityIndex();
	const bool retval = AbilityIndex > NO_ABILITY_INDEX;
	return (retval);
}

void AAbilityWeapon::InitResourceBindings(IResourceGatherer* InResourceSource)
{
	if (HasAuthority())
	{
		/*Todo filter to only relevant resources*/
		TArray<TSubclassOf<UResource>> resourcetypes = InResourceSource->GetSupportedResources();
		for (TSubclassOf<UResource> resourceclass : resourcetypes)
		{
			FOnResourceValueChangedDelegate& changedelegate = InResourceSource->BindResourceValueChangedEvent(resourceclass);
			changedelegate.AddUObject(this, &AAbilityWeapon::OnResourceSourceChanged);
		}
	}
}

void AAbilityWeapon::OnAbilityEnableStateChanged(UAbility * InSpawningAbility)
{
	if (AbilityIndex == NO_ABILITY_INDEX)
	{
		AbilityIndex = AbilityComp->GetNextAvailableIndex();
	}
	/*If the previewed ability is now disabled, cycle to a new ability*/
	else if (IsValid(InSpawningAbility) && !InSpawningAbility->IsAbilityEnabled() && InSpawningAbility == AbilityComp->GetAbilityByIndex(AbilityIndex))
	{
		AbilityIndex = AbilityComp->GetNextAvailableIndex();
	}
}

void AAbilityWeapon::OnResourceSourceChanged(const TSubclassOf<UResource> InClass, const uint32 InOldValue, const uint32 InNewValue, TScriptInterface<IResourceGatherer> InSource)
{
	if (AbilityComp->IsCasting() && AbilityComp->WantstoCast())
	{
		const UAbility * currentability = AbilityComp->GetCurrentAbility();
		const FReplicationResourceMap abilitycost = currentability->GetAbilityCost();
		if (!InSource->HasResource(abilitycost))
		{
			AbilityComp->InterruptAbility();
		}
	}
}

void AAbilityWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AbilityComp->AbilityChangeDelegate.AddDynamic(this, &AAbilityWeapon::OnAbilityEnableStateChanged);
}

void AAbilityWeapon::EndPlay(EEndPlayReason::Type InReason)
{
	AbilityComp->InterruptAbility();
	Super::EndPlay(InReason);
}

float AAbilityWeapon::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return MyPawn->TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
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