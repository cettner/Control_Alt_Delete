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

void UAbilityComponent::SetAbilityTarget(AActor * NewTarget, FHitResult AssociatedHit)
{
	if (GetOwner()->HasAuthority())
	{
		AbilityTarget = NewTarget;
	}
	LastAbilityHit = AssociatedHit;
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

void UAbilityComponent::InitAbilities(IAbilityUserInterface* InAbilitiyUser, TArray<TSubclassOf<UAbility>> InAllAbilityClasses)
{
	if (InAbilitiyUser != nullptr)
	{
		AbilityUser = InAbilitiyUser;

		for (int i = 0; i < InAllAbilityClasses.Num(); i++)
		{
			UAbility * newability =	NewObject<UAbility>(UAbility::StaticClass(), InAllAbilityClasses[i]);
			newability->Init(this);
			AllAbilites.Emplace(newability);
			const bool enabledstate = newability->GetDefaultEnabledState();
			EnabledAbilities.Emplace(enabledstate);
		}

		if (AllAbilites.Num() > 0)
		{
			bAbilitiesInitialized = EnabledAbilities.Num() == AllAbilites.Num();
			SetCurrentAbility(GetNextAvailableIndex(0, true));
		}
	}
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

void UAbilityComponent::InterruptAbility()
{
	if (IsCasting() && IsAbilityValid())
	{
		UAbility * ability = GetCurrentAbility();
		ability->OnAbilityInterrupted();
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

int UAbilityComponent::GetNextAvailableIndex(int InCurrentIndex, bool bOnlyEnabledAbilities) const
{
	int retval = NO_ABILITY_INDEX;
	const bool validindex = (AllAbilites.Num() > 0) && (InCurrentIndex >= NO_ABILITY_INDEX);

	if (bOnlyEnabledAbilities == true && validindex == true)
	{
		retval = GetNextEnabledIndex(InCurrentIndex);
	}
	else if (validindex)
	{
		retval = (InCurrentIndex + 1) % AllAbilites.Num();
	}

	return retval;
}

int UAbilityComponent::GetNextEnabledIndex(int StartIndex) const
{
	int retval = NO_ABILITY_INDEX;
	const int numabilities = AllAbilites.Num();

	int nextindex = (StartIndex + 1) % numabilities;
	bool abilityenabled = IsAbilityEnabled(nextindex);
	if (abilityenabled == true)
	{
		retval = nextindex;
	}

	int abilitycount = 0;
	while ((nextindex != StartIndex) && (retval == NO_ABILITY_INDEX) && (abilitycount < EnabledAbilities.Num()))
	{
		nextindex = (nextindex + 1) % numabilities;
		abilityenabled = IsAbilityEnabled(nextindex);

		if (abilityenabled == true)
		{
			retval = nextindex;
			break;
		}
		abilitycount++;
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

bool UAbilityComponent::IsAbilityEnabled(const int InIndex) const
{
	bool retval = false;
	const bool validindex = InIndex > NO_ABILITY_INDEX && InIndex < EnabledAbilities.Num();
	if (validindex == true)
	{
		retval = EnabledAbilities[InIndex];
	}
	return retval;
}

bool UAbilityComponent::SetAbilityEnabledState(const int InAbilityIndex, const bool InEnabledState)
{
	bool retval = false;
	TArray<bool> oldabilitystates = EnabledAbilities;
	if (HasAuthority() && IsAbilityValid(InAbilityIndex))
	{
		EnabledAbilities[InAbilityIndex] = InEnabledState;
		retval = true;
	}
	
	if (HasAuthority() && (GetNetMode() == NM_ListenServer))
	{
		OnRep_EnabledAbilities(oldabilitystates);
	}

	return(retval);
}

bool UAbilityComponent::SetAbilityEnabledState(TSubclassOf<UAbility> InEnabledClass, bool InEnabledState)
{
	const int index = GetAbilityIndexByClass(InEnabledClass);

	const bool retval = SetAbilityEnabledState(index, InEnabledState);

	return retval;
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

int UAbilityComponent::GetAbilityIndexByClass(TSubclassOf<UAbility> InEnabledClass) const
{
	int retval = NO_ABILITY_INDEX;
	if (InEnabledClass != nullptr)
	{
		for (int i = 0; i < AllAbilites.Num(); i++)
		{
			if (AllAbilites[i]->GetClass() == InEnabledClass)
			{
				retval = i;
				break;
			}
		}
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

bool UAbilityComponent::GetHitInfoFor(AActor * HitTarget, FHitResult & Hit)
{
	bool retval = false;
	if (LastAbilityHit.GetActor() == HitTarget)
	{
		Hit = LastAbilityHit;
		retval = true;
	}
	return retval;
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

bool UAbilityComponent::IsAbilityValid(int AbilityIndex) const
{
	return ((AbilityIndex > NO_ABILITY_INDEX) && (AbilityIndex < AllAbilites.Num()));
}

int UAbilityComponent::GetAbilityCost() const
{
	return 0;
}

IAbilityUserInterface * UAbilityComponent::GetAbilityUser() const
{
	return AbilityUser;
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
	AActor* spawnedactor = world->SpawnActorDeferred<AActor>(ActorClass, SpawnTransform, GetOwner(), Cast<APawn>(AbilityUser), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
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
	DOREPLIFETIME(UAbilityComponent, EnabledAbilities);
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

void UAbilityComponent::OnRep_EnabledAbilities(TArray<bool> PrevEnabledAbilities)
{
	const int checkindex = GetNextAvailableIndex();
	if (checkindex == NO_ABILITY_INDEX && !IsCasting())
	{
		/*If we have no avialable abilities, prevent user from attempting*/
		CurrentAbilityIndex = NO_ABILITY_INDEX;
	}
	else if (CurrentAbilityIndex == NO_ABILITY_INDEX)
	{
		/*If we dont have an ability Picked out, give the first one that shows up*/
		CurrentAbilityIndex = checkindex;
	}
	
	if (AbilityChangeDelegate.IsBound())
	{
		TArray<int> changedabilityindexs = TArray<int>();

		AbilityChangeDelegate.Broadcast(changedabilityindexs);
	}

}
