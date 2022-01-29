// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "Interfaces/AbilityUserInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UAbilityComponent::CanUseAbility(int AbilityIndex) const
{
	const IAbilityUserInterface * user = GetOwner<IAbilityUserInterface>();
	const UAbility * ability = GetAbilityByIndex(AbilityIndex);
	
	bool retval = IsValid(ability) && !IsCasting();
	if (user && retval)
	{
		retval = user->CanCastAbility();
	}
	else
	{
		retval = false;
	}


	return retval;
}

void UAbilityComponent::SetIsCasting(bool CastingState)
{
	if (HasAuthority())
	{
		bIsCasting.Set(CastingState, CurrentAbilityIndex);
	}
}

void UAbilityComponent::SetIsCastSuccessful(bool ReleaseState)
{
	if (HasAuthority())
	{
		bReleaseSuccess.Set(ReleaseState, CurrentAbilityIndex);
	}
}

void UAbilityComponent::SetAbilityTarget(AActor * NewTarget)
{
	if (GetOwner()->HasAuthority())
	{
		AbilityTarget = NewTarget;
	}
}

void UAbilityComponent::SetIsCastReady(bool ReadyState)
{
	bIsCastReady = ReadyState;
}

void UAbilityComponent::SetWantsToCast(bool InState)
{
	bWantstoCast = InState;
}

void UAbilityComponent::SetCurrentAbility(int InAbilityIndex)
{
	CurrentAbilityIndex = InAbilityIndex;
}

bool UAbilityComponent::HasAuthority()
{
	return GetOwner()->HasAuthority();
}

void UAbilityComponent::StartAbility(int InAbilityIndex)
{
	SetWantsToCast(true);

	if (CanUseAbility(InAbilityIndex))
	{
		SetCurrentAbility(InAbilityIndex);
		UAbility* abilitytostart = GetCurrentAbility();
		abilitytostart->OnAbilityStart();
	}
}

void UAbilityComponent::OnCastStart()
{
	SetIsCasting(true);
}

void UAbilityComponent::ReleaseAbility()
{
	SetWantsToCast(false);

	if (IsAbilityValid())
	{
		UAbility* abilitytorelease = GetCurrentAbility();
		abilitytorelease->OnAbilityReleased();
		SetIsCastReady(false);
	}

}

void UAbilityComponent::OnCastEnd()
{
	if (GetOwner()->HasAuthority())
	{
		SetIsCasting(false);
		if (WantstoCast())
		{
			StartAbility(CurrentAbilityIndex);
		}
	}
}

void UAbilityComponent::AbilityEffect()
{
	if (IsAbilityValid() && GetOwner()->HasAuthority())
	{
		GetCurrentAbility()->OnEffect();
	}
}

int UAbilityComponent::GetNextAvailableIndex(int InCurrentIndex, bool bOnlyEnabledAbilities)
{
	int retval = NO_ABILITY_INDEX;
	if (AllAbilites.Num() && (InCurrentIndex > NO_ABILITY_INDEX))
	{
		retval = (InCurrentIndex + 1) % AllAbilites.Num();
	}
	return retval;
}

void UAbilityComponent::OnReadyNotify()
{
	if (IsAbilityValid())
	{
		SetIsCastReady(true);
		UAbility* abilitytoready = GetCurrentAbility();
		abilitytoready->NotifyOnReady();
	}
}

void UAbilityComponent::OnLoopNotify()
{
	if (IsAbilityValid())
	{
		UAbility* abilitytoloop = GetCurrentAbility();
		abilitytoloop->NotifyOnLoop();
	}
}

void UAbilityComponent::OnEffectNotify()
{
	AbilityEffect();
}

void UAbilityComponent::OnEndNotify()
{
	EndAbility();
}

void UAbilityComponent::EndAbility()
{
	if (IsAbilityValid())
	{
		UAbility* abilitytoend = GetCurrentAbility();
		abilitytoend->OnAbilityEnd();
	}
}

UAbility * UAbilityComponent::GetCurrentAbility() const
{
	UAbility* retval = GetAbilityByIndex(CurrentAbilityIndex);

	return retval;
}

UAbility * UAbilityComponent::GetAbilityByIndex(int InIndex) const
{

	UAbility * retval = nullptr;
	if (CurrentAbilityIndex > NO_ABILITY_INDEX)
	{
		retval = AllAbilites[InIndex];
	}
	return retval;
}

int UAbilityComponent::GetCurrentAbilityIndex() const
{
	return CurrentAbilityIndex;
}

TArray<TWeakObjectPtr<UAbility>> UAbilityComponent::GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const
{
	TArray<TWeakObjectPtr<UAbility>> classabilities = TArray<TWeakObjectPtr<UAbility>>();
	for(int i = 0; i < AllAbilites.Num(); i++)
	{
		if(AllAbilites[i]->GetClass()->IsChildOf(AbilityClass))
		{
			classabilities.Emplace(TWeakObjectPtr<UAbility>(AllAbilites[i]));
		}
	}
	return(classabilities);
}

bool UAbilityComponent::IsAbilityUsingCrosshair() const
{
	return false;
}

bool UAbilityComponent::IsUsingAbility() const
{
	return false;
}

AActor * UAbilityComponent::GetAbilityTarget() const
{
	return AbilityTarget;
}

TWeakObjectPtr<UAbility> UAbilityComponent::AddAbility(TSubclassOf<UAbility> AbilityClass)
{
	UAbility * newability = nullptr;
	if (IsValid(AbilityClass))
	{
		newability = NewObject<UAbility>(UAbility::StaticClass(),AbilityClass);
		check(newability);
		newability->Init(this);
		AllAbilites.Emplace(newability);
		/*If we dont currently have an ability*/
		if(!IsAbilityValid())
		{
			SetCurrentAbility(0);
		}
	}
	return TWeakObjectPtr<UAbility>(newability);
}

bool UAbilityComponent::IsCasting() const
{
	return bIsCasting.WasSuccessful();
}

bool UAbilityComponent::WantstoCast() const
{
	return bWantstoCast;
}

bool UAbilityComponent::IsCastReady() const
{
	return bIsCastReady;
}

bool UAbilityComponent::IsAbilityValid() const
{
	const UAbility * ability = GetCurrentAbility();
	const bool retval = IsValid(ability);
	return retval;
}

int UAbilityComponent::GetAbilityCost() const
{
	return 0;
}

bool UAbilityComponent::ConsumeMana(int amount)
{
	return false;
}

IAbilityUserInterface * UAbilityComponent::GetAbilityUser() const
{
	return GetOwner<IAbilityUserInterface>();
}

float UAbilityComponent::PlayAbilityMontage(FAbilityAnim PlayAnim)
{
	IAbilityUserInterface * user = GetOwner<IAbilityUserInterface>();
	float playtime = -1.0f;

	if (user != nullptr)
	{
		playtime = user->PlayAbilityMontage(PlayAnim);
		CurrentMontage = PlayAnim;
	}


	return playtime;
}

bool UAbilityComponent::StopCurrentAnimation()
{
	bool retval = CurrentMontage.AnimFirstPerson != nullptr || CurrentMontage.AnimThirdPerson != nullptr;
	
	IAbilityUserInterface  * user = GetOwner<IAbilityUserInterface>();
	retval &= (user != nullptr);
	if (retval == true)
	{
		user->StopAbilityMontage(CurrentMontage);
	}
	CurrentMontage = FAbilityAnim();
	return retval;
}

AActor* UAbilityComponent::SpawnUninitializedActor(TSubclassOf<AActor> ActorClass, const FTransform &SpawnTransform)
{
	UWorld* world = GetWorld();
	AActor* spawnedactor = world->SpawnActorDeferred<AActor>(ActorClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	return spawnedactor;
}

AActor* UAbilityComponent::FinishSpawningActor(AActor* InitializedActor, const FTransform &SpawnTransform)
{
	return 	UGameplayStatics::FinishSpawningActor(InitializedActor, SpawnTransform);
}

FVector UAbilityComponent::GetControlRotation()
{
	return FVector();
}

FTransform UAbilityComponent::GetSurfaceTransform()
{
	return FTransform();
}

FTransform UAbilityComponent::GetCrosshairTransform(FName Socketname)
{
	IAbilityUserInterface* AbilityUser = GetOwner<IAbilityUserInterface>();
	FVector spawnlocation = FVector();
	FVector aimdirection = FVector();

	if (AbilityUser != nullptr)
	{
		spawnlocation = AbilityUser->GetAbilitySocketLocation(Socketname);
		aimdirection = AbilityUser->GetAbilityAimVector();
	}
	else
	{
		spawnlocation = GetOwner()->GetActorLocation();
		aimdirection = GetOwner()->GetActorForwardVector();
	}

	FTransform retval = FTransform(aimdirection.Rotation(), spawnlocation);

	return retval;
}

void UAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAbilityComponent, bIsCasting);
	DOREPLIFETIME(UAbilityComponent, AbilityTarget);
	DOREPLIFETIME(UAbilityComponent, bReleaseSuccess);
}

void UAbilityComponent::OnRep_bIsCasting()
{
	if (bIsCasting.WasSuccessful() == true)
	{
		const int abilityindex = bIsCasting.Index();

		if(abilityindex > NO_ABILITY_INDEX)
		{
			SetCurrentAbility(abilityindex);
			UAbility* ability = GetAbilityByIndex(abilityindex);
			ability->OnAbilityStart();
		}
	}
}

void UAbilityComponent::OnRep_bIsCastReleased()
{
	UAbility * castability = GetAbilityByIndex(bReleaseSuccess.Index());
	if (castability != nullptr)
	{
		if (bReleaseSuccess.WasSuccessful() == true)
		{
			bIsCastReady = true;
			castability->OnAbilityReleased();
		}
		else
		{
			bIsCastReady = false;
			castability->OnAbilityReleased();
		}
	}
}

void UAbilityComponent::OnRep_AbilityTarget()
{
	UAbility* currentability = GetCurrentAbility();
	if (currentability)
	{
		currentability->ProcessTarget(AbilityTarget);
	}
}