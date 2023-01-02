// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionSystem.h"
#include "Engine.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	//Make it so the component is "Alive"
	MaxHealth = 300.0f;
	CurrentHealth = MaxHealth;
}

void UHealthComponent::SetMaxHealth(float healthval, bool AdjustCurrentHealth)
{
	const float prevhealth = MaxHealth;
	MaxHealth = healthval;

	if (AdjustCurrentHealth == true)
	{
		if (CurrentHealth > MaxHealth)
		{
			SetCurrentHealth(MaxHealth);
		}
		else if (MaxHealth > prevhealth)
		{
			const float healthdiff = MaxHealth - prevhealth;
			const float newcurrenthealth = CurrentHealth + healthdiff;

			SetCurrentHealth(newcurrenthealth);
		}
	}
}

void UHealthComponent::SetDeathanimMontage(UAnimMontage* InMontage)
{
	DeathAnimMontage = InMontage;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetCurrentHealth() const 
{
	return CurrentHealth;
}

void UHealthComponent::SetCurrentHealth(float InHealth)
{
	CurrentHealth = InHealth;

	if (CurrentHealth <= 0.0f)
	{
		Die(0.0f, FDamageEvent(), nullptr, nullptr);
	}
}

float UHealthComponent::GetHealthPercentage() const
{
	if (MaxHealth <= 0.0) return(0.0);
	float percent = CurrentHealth / MaxHealth;
	percent = FMath::Clamp(percent, 0.0f, 1.0f);

	return (percent);
}

bool UHealthComponent::IsAlive() const
{
	return (CurrentHealth > 0.0F);
}

float UHealthComponent::HandleDamageEvent(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float ActualDamage = Damage;
	if (Damage > 0.0f && !bIsDying)
	{
		ActualDamage = ModifyDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		CurrentHealth -= ActualDamage;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s: Damage Taken: %.2f Health Remaining: %.2f"),*CompOwner->GetName(), ActualDamage, CurrentHealth));
		if (CurrentHealth <= 0.0f)
		{
			Die(Damage, DamageEvent, EventInstigator, DamageCauser);
		}
		else
		{

		}
		
		// Make Some Noise if the owner happens to be a Pawn
		if (Cast<APawn>(CompOwner))
		{
			APawn * NoiseMaker = Cast<APawn>(CompOwner);
			float NoiseRange = 300.0f;
			CompOwner->MakeNoise(1.0f, EventInstigator ? EventInstigator->GetPawn() : NoiseMaker);
		}
	}

	return (ActualDamage);
}

bool UHealthComponent::CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const
{
	bool InevitabilityOfDeath;

	if (bIsDying || !IsValid(CompOwner) || (CompOwner->GetLocalRole() != ROLE_Authority))
	{
		InevitabilityOfDeath = false;
	}
	else
	{
		InevitabilityOfDeath = true;
	}
	
	return(InevitabilityOfDeath);
}

bool UHealthComponent::Die(float KillingDamage, FDamageEvent const & DamageEvent, AController * Killer, AActor * DamageCauser)
{
	if (!CanDie(KillingDamage, DamageEvent, Killer, DamageCauser))
	{
		return (false);
	}
	CurrentHealth = FMath::Min(0.0f, CurrentHealth);

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);

	return (true);
}

void UHealthComponent::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* InstigatingPawn, AActor* DamageCauser)
{
	ACharacter* CharacterOwner = Cast<ACharacter>(CompOwner);
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(CompOwner->FindComponentByClass(USkeletalMeshComponent::StaticClass()));

	if (bIsDying)
	{
		return;
	}

	OnDeathStart.ExecuteIfBound();

	bIsDying = true;
	
	if (CompOwner->GetLocalRole() == ROLE_Authority)
	{
		ReplicateHit(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser, true);
	}

	if (CharacterOwner)
	{
		//Initial Character Tear Down
		CharacterOwner->DetachFromControllerPendingDestroy();
		CharacterOwner->SetReplicatingMovement(false);
		CharacterOwner->TearOff();
		CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterOwner->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	// Death anim
	float DeathAnimDuration = 0.0f;
	if (SkeletalMeshComp && DeathAnimMontage)
	{
		DeathAnimDuration = SkeletalMeshComp->GetAnimInstance()->Montage_Play(DeathAnimMontage);
		/*Delete Actor just before the animation ends*/
		DeathAnimDuration -= .6f;
	}

	if (bShouldRagdoll && SkeletalMeshComp)
	{
		HandleRagDoll(SkeletalMeshComp, DeathAnimDuration);
	}
	else
	{
		DestroyOwner(DeathAnimDuration);
	}
}

void UHealthComponent::SetRagDollPhysics(USkeletalMeshComponent* Mesh, UMovementComponent* Movement)
{
	//Try to cast to a Character if no mesh or movement is provided
	if (Cast<ACharacter>(CompOwner) && (!Mesh || !Movement))
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(CompOwner);
		if (!Mesh)
		{
			Mesh = CharacterOwner->GetMesh();
		}
		if (!Movement)
		{
			Movement = CharacterOwner->GetCharacterMovement();
		}
	}

	//Shut down Movement Component functionality
	if (Movement)
	{
		Movement->StopMovementImmediately();
		if (Cast<UCharacterMovementComponent>(Movement))
		{
			Cast<UCharacterMovementComponent>(Movement)->DisableMovement();
		}
		Movement->SetComponentTickEnabled(false);
	}

	//Enable Ragdoll Physics
	if (Mesh && Mesh->GetPhysicsAsset())
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetSimulatePhysics(true);
		Mesh->WakeAllRigidBodies();
		Mesh->bBlendPhysics = true;
	}


	if (bDestroyAfterRagdoll == true)
	{
		//Ragdoll Enabled, set time till Actor Destruction
		DestroyOwner(TimeRagdoll);
	}
}

void UHealthComponent::DestroyOwner(float timetilldestruction)
{
		if (timetilldestruction > 0.0F)
		{
			CompOwner->SetLifeSpan(timetilldestruction);
		}
		else
		{
			CompOwner->Destroy();
		}
}

void UHealthComponent::ReplicateHit(float Damage, FDamageEvent const & DamageEvent, APawn * InstigatingPawn, AActor * DamageCauser, bool bKilled)
{
	if (CompOwner->GetLocalRole() == ROLE_Authority)
	{
		const float TimeoutTime = GetWorld()->GetTimeSeconds() + 0.5f;

		if ((InstigatingPawn == LastTakeHitInfo.PawnInstigator.Get()) && (DamageEvent.DamageTypeClass == LastTakeHitInfo.DamageTypeClass) && (LastTakeHitTimeTimeout == TimeoutTime))
		{
			// same frame damage
			if (bKilled && LastTakeHitInfo.bKilled)
			{
				// Redundant death take hit, just ignore it
				return;
			}

			// otherwise, accumulate damage done this frame
			Damage += LastTakeHitInfo.ActualDamage;
		}

		LastTakeHitInfo.ActualDamage = Damage;
		LastTakeHitInfo.PawnInstigator = InstigatingPawn;
		LastTakeHitInfo.DamageCauser = DamageCauser;
		LastTakeHitInfo.bKilled = bKilled;
		LastTakeHitInfo.DamageTypeClass = DamageEvent.DamageTypeClass;
		LastTakeHitInfo.EnsureReplication();

		LastTakeHitTimeTimeout = TimeoutTime;
	}
}

void UHealthComponent::PlayLocalHit(float Damage, FDamageEvent const& DamageEvent, APawn* InstigatingPawn, AActor* DamageCauser)
{
	if (CompOwner->GetLocalRole() == ROLE_Authority)
	{
		ReplicateHit(Damage, DamageEvent, InstigatingPawn, DamageCauser, false);
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CompOwner = GetOwner();
	if (!CompOwner)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("HealthComponent Initialization Failed!")));
	}
}

float UHealthComponent::ModifyDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return(Damage);
}

USkeletalMeshComponent* UHealthComponent::GetOwnerMesh()
{
	return nullptr;
}

UMovementComponent* UHealthComponent::GetOwnerMovement()
{
	return nullptr;
}

void UHealthComponent::OnRep_LastTakeHitInfo()
{
	FDamageEvent Classtype = FDamageEvent();
	Classtype.DamageTypeClass = LastTakeHitInfo.DamageTypeClass;
	if (LastTakeHitInfo.bKilled)
	{
		OnDeath(LastTakeHitInfo.ActualDamage, Classtype, LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
	else
	{
		PlayLocalHit(LastTakeHitInfo.ActualDamage, Classtype, LastTakeHitInfo.PawnInstigator.Get(), LastTakeHitInfo.DamageCauser.Get());
	}
}

void UHealthComponent::PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	// Only replicate this property for a short duration after it changes so join in progress players don't get spammed with fx when joining late
	DOREPLIFETIME_ACTIVE_OVERRIDE(UHealthComponent, LastTakeHitInfo, GetWorld() && GetWorld()->GetTimeSeconds() < LastTakeHitTimeTimeout);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
	DOREPLIFETIME(UHealthComponent, MaxHealth);
	DOREPLIFETIME_CONDITION(UHealthComponent, LastTakeHitInfo, COND_Custom);
}

void UHealthComponent::HandleRagDoll(USkeletalMeshComponent * Mesh, const float DeathAnimDuration)
{
	// Ragdoll
	if (DeathAnimDuration > 0.f && GetWorld())
	{
		// Trigger ragdoll a little before the animation early so the character doesn't
		// blend back to its normal position.
		const float TriggerRagdollTime = DeathAnimDuration - 0.7f;

		// Enable blend physics so the bones are properly blending against the montage.
		Mesh->bBlendPhysics = true;

		// Set a Timer to to Enable ragdoll after death animation has just about completed;
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHealthComponent::RagDollTimerHandler, FMath::Max(0.1f, TriggerRagdollTime), false);
	}
	else
	{
		//Go straight to Ragdoll
		SetRagDollPhysics();
	}
}

void UHealthComponent::RagDollTimerHandler()
{
	SetRagDollPhysics();
}




