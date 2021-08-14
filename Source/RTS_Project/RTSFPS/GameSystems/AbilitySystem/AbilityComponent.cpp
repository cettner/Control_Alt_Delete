// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "Interfaces/AbilityUserInterface.h"

#include "Kismet/GameplayStatics.h"

#define DEBUG_WEAPON

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

void UAbilityComponent::OnCastStart()
{
	SetIsCasting(true);
}

void UAbilityComponent::ReleaseAbility()
{
	SetWantsToCast(false);
	if (IsAbilityValid())
	{
		CurrentAbility->OnAbilityReleased();
	}

}

void UAbilityComponent::OnCastEnd()
{
	SetIsCasting(false);
	if (WantstoCast())
	{
		StartAbility();
	}
}

void UAbilityComponent::AbilityEffect()
{
	if (IsAbilityValid())
	{
		CurrentAbility->OnEffect();
	}
}

void UAbilityComponent::ChangeAbility()
{

}

void UAbilityComponent::OnReadyNotify()
{
	if (IsAbilityValid())
	{
		CurrentAbility->NotifyOnReady();
	}
}

void UAbilityComponent::OnLoopNotify()
{
	if (IsAbilityValid())
	{
		CurrentAbility->NotifyOnLoop();
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
		CurrentAbility->OnAbilityEnd();
	}
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
	const FTransform reference = SpawnTransform;
	AActor * spawnedactor = world->SpawnActorDeferred<AActor>(ActorClass, SpawnTransform, GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	
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
	const FCollisionShape traceshape = FCollisionShape::MakeCapsule(1.0, 1.0);
	FCollisionQueryParams queryparams = FCollisionQueryParams::DefaultQueryParam;

	if (AbilityUser != nullptr)
	{
		spawnlocation = AbilityUser->GetAbilitySocketLocation(Socketname);
		aimdirection = AbilityUser->GetAbilityAimVector();
		queryparams.AddIgnoredActors(AbilityUser->GetIgnoredTraceActors());
	}
	else
	{
		spawnlocation = GetOwner()->GetActorLocation();
		aimdirection = GetOwner()->GetActorForwardVector();
		queryparams.AddIgnoredActor(GetOwner());
	}
	
	FVector starttrace = spawnlocation;
	FVector endtrace = starttrace + (aimdirection * 1000.0f);


	#ifdef DEBUG_WEAPON
		UWorld * world = GetWorld();
		FHitResult outhit;
		const FName TraceTag("DebugShooterTag");
		world->DebugDrawTraceTag = TraceTag;
		queryparams.TraceTag = TraceTag;
		world->SweepSingleByChannel(outhit, starttrace, endtrace, aimdirection.ToOrientationQuat(), CurrentAbility->GetAbilityCollisionChannel(), traceshape, queryparams);
	#endif // Define to view weapon trace


	FTransform retval = FTransform(aimdirection.Rotation(), spawnlocation);

	return retval;
}
