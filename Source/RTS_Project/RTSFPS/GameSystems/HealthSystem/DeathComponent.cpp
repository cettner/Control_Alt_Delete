// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathComponent.h"
#include "Net/UnrealNetwork.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"




// Sets default values for this component's properties
UDeathComponent::UDeathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UDeathComponent::SetDeathanimMontage(UAnimMontage* InMontage)
{
	DeathAnimMontage = InMontage;
}


bool UDeathComponent::IsAlive() const
{
	return !bIsDying;
}

bool UDeathComponent::CanDie() const
{
	bool InevitabilityOfDeath;

	if (bIsDying || !IsValid(GetOwner()) || (GetOwner()->GetLocalRole() != ROLE_Authority))
	{
		InevitabilityOfDeath = false;
	}
	else
	{
		InevitabilityOfDeath = true;
	}
	
	return(InevitabilityOfDeath);
}

bool UDeathComponent::Die()
{
	if (!CanDie() || bIsDying || !GetOwner()->HasAuthority())
	{
		return (false);
	}

	OnDeath();

	return (true);
}

void UDeathComponent::OnDeath()
{
	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(GetOwner()->FindComponentByClass(USkeletalMeshComponent::StaticClass()));

	OnDeathStart.ExecuteIfBound();

	bIsDying = true;

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

void UDeathComponent::SetRagDollPhysics(USkeletalMeshComponent* Mesh, UMovementComponent* Movement)
{
	//Try to cast to a Character if no mesh or movement is provided
	if (Cast<ACharacter>(GetOwner()) && (!Mesh || !Movement))
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
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

void UDeathComponent::DestroyOwner(float timetilldestruction)
{
		if (timetilldestruction > 0.0F)
		{
			GetOwner()->SetLifeSpan(timetilldestruction);
		}
		else
		{
			GetOwner()->Destroy();
		}
}

USkeletalMeshComponent* UDeathComponent::GetOwnerMesh()
{
	return nullptr;
}

UMovementComponent* UDeathComponent::GetOwnerMovement()
{
	return nullptr;
}

void UDeathComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDeathComponent, bIsDying);
}

void UDeathComponent::OnRep_bIsDying()
{
	if (bIsDying)
	{
		OnDeath();
	}
}

void UDeathComponent::HandleRagDoll(USkeletalMeshComponent * Mesh, const float DeathAnimDuration)
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
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UDeathComponent::RagDollTimerHandler, FMath::Max(0.1f, TriggerRagdollTime), false);
	}
	else
	{
		//Go straight to Ragdoll
		SetRagDollPhysics();
	}
}

void UDeathComponent::RagDollTimerHandler()
{
	SetRagDollPhysics();
}




