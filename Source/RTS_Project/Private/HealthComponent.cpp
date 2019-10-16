// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	if (MaxHealth > 0.0F)
	{
		Current_Health = MaxHealth;
	}
	else
	{
		//Make it so the component is "Alive"
		Current_Health = 1.0F;
	}
}

void UHealthComponent::SetMaxHealth(float healthval)
{
	MaxHealth = healthval;
}

bool UHealthComponent::IsAlive()
{
	return (Current_Health > 0.0F);
}

float UHealthComponent::HandleDamageEvent(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float ActualDamage = Damage;
	if (Damage > 0.0f)
	{
		ActualDamage = ModifyDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		Current_Health -= ActualDamage;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("%s: Damage Taken: %.2f Health Remaining: %.2f"),*CompOwner->GetName(), ActualDamage, Current_Health));
		if (Current_Health <= 0.0f)
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

	if (bIsDying || (CompOwner && CompOwner->IsPendingKill()) || (CompOwner && CompOwner->Role != ROLE_Authority))
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
	Current_Health = FMath::Min(0.0f, Current_Health);

	OnDeath(KillingDamage, DamageEvent, Killer ? Killer->GetPawn() : NULL, DamageCauser);

	return (true);
}

void UHealthComponent::OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* InstigatingPawn, AActor* DamageCauser)
{
	ACharacter* CharacterOwner = Cast<ACharacter>(CompOwner);
	if (bIsDying)
	{
		return;
	}

	if (CharacterOwner)
	{
		//Initial Character Tear Down
		CharacterOwner->bReplicateMovement = false;
		CharacterOwner->TearOff();
		CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterOwner->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	bIsDying = true;

	if (CompOwner->Role == ROLE_Authority)
	{
		ReplicateHit(KillingDamage, DamageEvent, InstigatingPawn, DamageCauser, true);
	}

	// Death anim
	float DeathAnimDuration = 0.0f;
	if (CharacterOwner && DeathAnim)
	{
		DeathAnimDuration = CharacterOwner->PlayAnimMontage(DeathAnim);
	}

	if (ShouldRagdoll && CharacterOwner && CharacterOwner->GetMesh())
	{
		HandleRagDoll(CharacterOwner->GetMesh(), DeathAnimDuration);
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

	bool bInRagdoll = false;

	//Enable Ragdoll Physics
	if (CompOwner->IsPendingKill() || !Mesh || !Mesh->GetPhysicsAsset())
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->WakeAllRigidBodies();
		Mesh->bBlendPhysics = true;

		bInRagdoll = true;
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
		
		bInRagdoll = true;
	}

	if (!bInRagdoll)
	{
		// hide and set short lifespan
		if (Cast<APawn>(CompOwner))
		{
			Cast<APawn>(CompOwner)->TurnOff();
		}
		CompOwner->SetActorHiddenInGame(true);
		CompOwner->SetLifeSpan(1.0f);
	}
	else
	{
		CompOwner->SetLifeSpan(TimeRagdoll);
	}
}

void UHealthComponent::ReplicateHit(float Damage, FDamageEvent const & DamageEvent, APawn * InstigatingPawn, AActor * DamageCauser, bool bKilled)
{
	const float TimeoutTime = GetWorld()->GetTimeSeconds() + 0.5f;

	if ((InstigatingPawn == LastTakeHitInfo.PawnInstigator.Get()) && (LastHitTypeClass == LastTakeHitInfo.DamageTypeClass) && (LastTakeHitTimeTimeout == TimeoutTime))
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
	LastTakeHitInfo.EnsureReplication();

	LastTakeHitTimeTimeout = TimeoutTime;
	LastHitTypeClass = LastTakeHitInfo.DamageTypeClass;
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

void UHealthComponent::OnRep_LastTakeHitInfo()
{
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UHealthComponent,Current_Health)
}

void UHealthComponent::HandleRagDoll(USkeletalMeshComponent * Mesh, const float DeathAnimDuration)
{
	// Ragdoll
	if (DeathAnimDuration > 0.f && ShouldRagdoll && GetWorld())
	{
		// Trigger ragdoll a little before the animation early so the character doesn't
		// blend back to its normal position.
		const float TriggerRagdollTime = DeathAnimDuration - 0.7f;

		// Enable blend physics so the bones are properly blending against the montage.
		Mesh->bBlendPhysics = true;

		// Set a Timer to to Enable ragdoll
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHealthComponent::RagDollTimerHandler, FMath::Max(0.1f, TriggerRagdollTime), false);
	}
	else if(ShouldRagdoll)
	{
		SetRagDollPhysics();
	}
	else
	{

	}
}

void UHealthComponent::RagDollTimerHandler()
{
	SetRagDollPhysics();
}




