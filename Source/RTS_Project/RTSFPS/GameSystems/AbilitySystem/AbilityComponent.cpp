// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "Interfaces/AbilityUserInterface.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

bool UAbilityComponent::CanUseAbility() const
{
	IAbilityUserInterface * user = GetOwner<IAbilityUserInterface>();
	bool retval = IsAbilityValid() && !IsCasting();
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
	bIsCasting = CastingState;
}

void UAbilityComponent::SetWantsToCast(bool InState)
{
	bWantstoCast = InState;
}

void UAbilityComponent::SetCurrentAbility(UAbility * InAbility)
{
	CurrentAbility = InAbility;
}

void UAbilityComponent::StartAbility()
{
	SetWantsToCast(true);
	if (CanUseAbility())
	{
		CurrentAbility->OnAbilityStart();
	}
}

void UAbilityComponent::OnCastNotify()
{
	SetIsCasting(true);
}

void UAbilityComponent::ReleaseAbility()
{
	if (IsAbilityValid() && WantstoCast())
	{
		CurrentAbility->OnAbilityReleased();
	}
	SetWantsToCast(false);
}

void UAbilityComponent::AbilityEffect()
{

}

void UAbilityComponent::ChangeAbility()
{

}

void UAbilityComponent::OnReadyNotify()
{
}

void UAbilityComponent::OnLoopNotify()
{
}

void UAbilityComponent::EndAbility()
{

}

TWeakObjectPtr<UAbility> UAbilityComponent::GetCurrentAbility() const
{
	return CurrentAbility;
}

bool UAbilityComponent::IsAbilityUsingCrosshair() const
{
	return false;
}

bool UAbilityComponent::IsUsingAbility() const
{
	return false;
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
			SetCurrentAbility(newability);
		}
	}
	return TWeakObjectPtr<UAbility>(newability);
}

bool UAbilityComponent::IsCasting() const
{
	return bIsCasting;
}

bool UAbilityComponent::WantstoCast() const
{
	return bWantstoCast;
}

bool UAbilityComponent::IsAbilityValid() const 
{
	bool retval = IsValid(CurrentAbility);
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

float UAbilityComponent::PlayAbilityMontage(FAbilityAnim PlayAnim)
{
	IAbilityUserInterface * user = GetOwner<IAbilityUserInterface>();
	if (user != nullptr)
	{
		user->PlayAbilityMontage(PlayAnim);
	}


	return -1.0f;
}

FVector UAbilityComponent::GetControlRotation()
{
	return FVector();
}

FTransform UAbilityComponent::GetSurfaceTransform()
{
	return FTransform();
}
