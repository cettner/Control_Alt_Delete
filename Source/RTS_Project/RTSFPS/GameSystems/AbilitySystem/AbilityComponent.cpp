// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "Interfaces/AbilityUserInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bReplicateUsingRegisteredSubObjectList = true;
}

bool UAbilityComponent::CanUseAbility(int AbilityIndex) const
{
	const IAbilityUserInterface * user = GetOwner<IAbilityUserInterface>();
	const TWeakObjectPtr<UAbility> ability = GetAbilityByIndex(AbilityIndex);
	
	bool retval = IsValid(ability.Get()) && !IsCasting();
	if (user && retval)
	{
		retval = user->CanCastAbility(ability);
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

void UAbilityComponent::InitAbilities(IAbilityUserInterface* InAbilitiyUser)
{
	if (InAbilitiyUser != nullptr)
	{
		AbilityUser = InAbilitiyUser;
		if (HasAuthority())
		{
			for (const TSubclassOf<UAbility>& abilityclass : AbilityClasses)
			{
				UAbility* newability = NewObject<UAbility>(UAbility::StaticClass(), abilityclass);
				AddReplicatedSubObject(newability);
				newability->Init(this);
				AllAbilites.Emplace(newability);
			}

			if (AllAbilites.Num() > 0)
			{
				SetCurrentAbility(GetNextAvailableIndex(NO_ABILITY_INDEX, true));
			}
		}
	}
}

bool UAbilityComponent::HasAuthority() const
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
	if (HasAuthority())
	{
		SetIsCasting(false);
		if (WantstoCast())
		{
			StartAbility(CurrentAbilityIndex);
		}
	}
}

void UAbilityComponent::OnAbilityEnableStateChanged(UAbility* InAbility)
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
		AbilityChangeDelegate.Broadcast(InAbility);
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
	bool abilityenabled = AllAbilites[nextindex]->IsAbilityEnabled();
	
	if (abilityenabled == true)
	{
		retval = nextindex;
	}

	int abilitycount = 0;
	while ((nextindex != StartIndex) && (retval == NO_ABILITY_INDEX) && (abilitycount < AllAbilites.Num()))
	{
		nextindex = (nextindex + 1) % numabilities;
		abilityenabled = AllAbilites[nextindex]->IsAbilityEnabled();

		if (abilityenabled == true)
		{
			retval = nextindex;
			break;
		}
		abilitycount++;
	}

	return retval;
}

void UAbilityComponent::OnReadyNotify(UAbilityAnimNotify* CallingContext)
{
	if (IsAbilityValid())
	{
		SetIsCastReady(true);
		UAbility* abilitytoready = GetCurrentAbility();
		abilitytoready->NotifyOnReady();
	}
}

void UAbilityComponent::OnLoopNotify(UAbilityAnimNotify* CallingContext)
{
	if (IsAbilityValid())
	{
		UAbility* abilitytoloop = GetCurrentAbility();
		abilitytoloop->NotifyOnLoop();
	}
}

void UAbilityComponent::OnEffectNotify(UAbilityAnimNotify* CallingContext)
{
	AbilityEffect();
}

void UAbilityComponent::OnEndNotify(UAbilityAnimNotify* CallingContext)
{
	EndAbility();
}

UAbility* UAbilityComponent::GetNextEnabledAbility(const UAbility* InIterator) const
{
	UAbility* retval = nullptr;
	int index = NO_ABILITY_INDEX;

	if (InIterator != nullptr)
	{
		index = GetAbilityIndexByClass(InIterator->GetClass());
		index = GetNextEnabledIndex(index);
	}
	else
	{
		index = GetNextEnabledIndex(NO_ABILITY_INDEX);
	}

	if (index != NO_ABILITY_INDEX)
	{
		retval = AllAbilites[index];
	}

	return retval;
}

TSet<TSubclassOf<UAbility>> UAbilityComponent::GetSupportedAbilities() const
{
	return AbilityClasses;
}

bool UAbilityComponent::SupportsAbility(const TSubclassOf<UAbility>& Inabilityclass) const
{
	return AbilityClasses.Contains(Inabilityclass);
}

void UAbilityComponent::EnableAbility(const TSubclassOf<UAbility>& InAbilityClass)
{
	SetAbilityEnabledState(InAbilityClass, true);
}

bool UAbilityComponent::DisableAbility(const TSubclassOf<UAbility>& InAbilityClass)
{
	const bool retval = SetAbilityEnabledState(InAbilityClass, false);
	return retval;
}

bool UAbilityComponent::IsAbilityEnabled(const TSubclassOf<UAbility>& InAbilityClass) const
{
	const int index = GetAbilityIndexByClass(InAbilityClass);
	const bool retval = IsAbilityEnabled(index);
	return retval;
}

TArray<TWeakObjectPtr<UAbility>> UAbilityComponent::GetAbilitiesByClass(const TSubclassOf<UAbility>& AbilityClass) const
{
	TArray<TWeakObjectPtr<UAbility>> classabilities = TArray<TWeakObjectPtr<UAbility>>();
	for (int i = 0; i < AllAbilites.Num(); i++)
	{
		if (AllAbilites[i]->GetClass()->IsChildOf(AbilityClass))
		{
			classabilities.Emplace(TWeakObjectPtr<UAbility>(AllAbilites[i]));
		}
	}
	return(classabilities);
}

bool UAbilityComponent::IsAbilityEnabled(const int InIndex) const
{
	bool retval = false;
	const bool validindex = InIndex > NO_ABILITY_INDEX && InIndex < AllAbilites.Num();
	if (validindex == true)
	{
		retval = AllAbilites[InIndex]->IsAbilityEnabled();
	}
	return retval;
}

bool UAbilityComponent::SetAbilityEnabledState(const int InAbilityIndex, const bool InEnabledState)
{
	bool retval = false;
	if (HasAuthority() && IsAbilityValid(InAbilityIndex))
	{
		AllAbilites[InAbilityIndex]->SetIsAbilityEnabled(InEnabledState);
		retval = true;
	}

	return(retval);
}

bool UAbilityComponent::SetAbilityEnabledState(const TSubclassOf<UAbility>& InEnabledClass, bool InEnabledState)
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

int UAbilityComponent::GetAbilityIndexByClass(const TSubclassOf<UAbility>& InAbilityClass) const
{
	int retval = NO_ABILITY_INDEX;
	if (InAbilityClass != nullptr)
	{
		for (int i = 0; i < AllAbilites.Num(); i++)
		{
			if (AllAbilites[i]->GetClass() == InAbilityClass)
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

IAbilityUserInterface * UAbilityComponent::GetAbilityUser() const
{
	return AbilityUser;
}

float UAbilityComponent::PlayAbilityMontage(const FAbilityAnim& AnimToPlay)
{
	IAbilityUserInterface * user = GetOwner<IAbilityUserInterface>();
	float playtime = -1.0f;

	if (user != nullptr)
	{
		playtime = user->PlayAbilityMontage(AnimToPlay);
		CurrentMontage = AnimToPlay;
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
	return UGameplayStatics::FinishSpawningActor(InitializedActor, SpawnTransform);
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

	const FTransform retval = FTransform(aimdirection.Rotation(), spawnlocation);

	return retval;
}

void UAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAbilityComponent, bIsCasting);
	DOREPLIFETIME(UAbilityComponent, bReleaseSuccess);
	DOREPLIFETIME(UAbilityComponent, AllAbilites);
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

void UAbilityComponent::OnRep_AllAbilities()
{
	for (int i = 0; i < AllAbilites.Num(); i++)
	{
		if (!AllAbilites[i]->IsInitialized())
		{
			AllAbilites[i]->Init(this);
			AbilityChangeDelegate.Broadcast(AllAbilites[i]);
		}
	}

	if (AllAbilites.Num() > 0)
	{
		const int currentindex = GetNextAvailableIndex(NO_ABILITY_INDEX, true);
		SetCurrentAbility(currentindex);
	}
}
