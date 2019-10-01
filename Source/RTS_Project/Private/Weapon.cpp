// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkinnedMeshComponent.h"
#include "UnrealNetwork.h"
#include "CombatCommander.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"




// Sets default values
AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	CollisionComp =  CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = CollisionComp;

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Weapon Mesh"));
	FirstPersonMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	FirstPersonMesh->bReceivesDecals = false;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetCollisionObjectType(ECC_WorldDynamic);
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FirstPersonMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	FirstPersonMesh->SetupAttachment(RootComponent);

	
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Third Person Weapon Mesh"));
	ThirdPersonMesh->bReceivesDecals = false;
	ThirdPersonMesh->CastShadow = true;
	ThirdPersonMesh->SetupAttachment(RootComponent);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

}

Weapon_Grip_Type AWeapon::GetType()
{
	return(Grip_Type);
}

void AWeapon::OnEquip(const AWeapon * LastWeapon)
{
	AttachMeshToPawn();
	bPendingEquip = true;
	DetermineWeaponState();

	// Only play animation if last weapon is valid
	if (LastWeapon)
	{
		float Duration = PlayWeaponAnimation(EquipAnim);
		if (Duration <= 0.0f)
		{
			// failsafe
			Duration = 0.5f;
		}

		GetWorldTimerManager().SetTimer(TimerHandle_OnEquipFinished, this, &AWeapon::OnEquipFinished, Duration, false);
	}
	else
	{
		OnEquipFinished();
	}

	if (MyPawn && MyPawn->IsLocallyControlled())
	{
		PlayWeaponSound(EquipSound);
	}
}

void AWeapon::OnEquipFinished()
{
	AttachMeshToPawn();

	bIsEquipped = true;

	bPendingEquip = false;

	// Determine the state so that the can reload checks will work
	DetermineWeaponState();
}

void AWeapon::OnUnEquip(const AWeapon* NextWeapon)
{
	if (bPendingEquip)
	{
		StopWeaponAnimation(EquipAnim);
		bPendingEquip = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_OnEquipFinished);
	}

	if (!bPendingUnEquip && bIsEquipped)
	{
		bPendingUnEquip = true;
		float Duration = PlayWeaponAnimation(UnEquipAnim);
		if (Duration <= 0.0f)
		{
			// failsafe
			Duration = 0.5f;
		}
		GetWorldTimerManager().SetTimer(TimerHandle_OnUnEquipFinished, this, &AWeapon::OnUnEquipFinished, Duration, false);
	}

	DetermineWeaponState();
}

void AWeapon::OnUnEquipFinished()
{
	DetachMeshFromPawn();
	bIsEquipped = false;
	bPendingUnEquip = false;
	DetermineWeaponState();
}

void AWeapon::OnEnterInventory(ACombatCommander * NewOwner)
{
	SetOwningPawn(NewOwner);
}

void AWeapon::OnLeaveInventory()
{
	if (Role == ROLE_Authority)
	{
		SetOwningPawn(NULL);
	}

	if (IsAttachedToPawn())
	{
		OnUnEquip();
	}
}

void AWeapon::SetOwningPawn(ACombatCommander* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
		
		// net owner for RPC calls
		SetOwner(NewOwner);
	}
}

bool AWeapon::IsAttachedToPawn() const
{
	return(bIsEquipped || bPendingEquip);
}

void AWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		// Remove and hide both first and third person meshes
		DetachMeshFromPawn();

		// For locally controller players we attach both weapons and let the bOnlyOwnerSee, bOwnerNoSee flags deal with visibility.

		FName AttachPoint = MyPawn->GetWeaponAttachPoint(this);

		if (MyPawn->IsLocallyControlled() == true)
		{
			USkeletalMeshComponent* PawnMesh1p = MyPawn->GetSpecifcPawnMesh(true);
			USkeletalMeshComponent* PawnMesh3p = MyPawn->GetSpecifcPawnMesh(false);
			FirstPersonMesh->SetHiddenInGame(false);
			ThirdPersonMesh->SetHiddenInGame(false);
			FirstPersonMesh->AttachToComponent(PawnMesh1p, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
			ThirdPersonMesh->AttachToComponent(PawnMesh3p, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
		}
		else
		{
			USkeletalMeshComponent* UseWeaponMesh = GetWeaponMesh();
			USkeletalMeshComponent* UsePawnMesh = MyPawn->GetPawnMesh();
			UseWeaponMesh->AttachToComponent(UsePawnMesh, FAttachmentTransformRules::KeepRelativeTransform, AttachPoint);
			UseWeaponMesh->SetHiddenInGame(false);
		}
	}
}

void AWeapon::DetachMeshFromPawn()
{
	FirstPersonMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	FirstPersonMesh->SetHiddenInGame(true);

	ThirdPersonMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	ThirdPersonMesh->SetHiddenInGame(true);
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* Sound)
{
	UAudioComponent* AC = NULL;
	if (Sound && MyPawn)
	{
		AC = UGameplayStatics::SpawnSoundAttached(Sound, MyPawn->GetRootComponent());
	}

	return AC;
}

float AWeapon::PlayWeaponAnimation(const FWeaponAnim& Animation)
{
	float Duration = 0.0f;
	if (MyPawn)
	{
		UAnimMontage* UseAnim = MyPawn->IsFirstPerson() ? Animation.AnimFirstPerson : Animation.AnimThirdPerson;
		if (UseAnim)
		{
			Duration = MyPawn->PlayAnimMontage(UseAnim);
		}
	}

	return Duration;
}

void AWeapon::StopWeaponAnimation(const FWeaponAnim& Animation)
{
	if (MyPawn)
	{
		UAnimMontage* UseAnim = MyPawn->IsFirstPerson() ? Animation.AnimFirstPerson : Animation.AnimThirdPerson;
		if (UseAnim)
		{
			MyPawn->StopAnimMontage(UseAnim);
		}
	}
}

void AWeapon::DetermineWeaponState()
{
	EWeaponState::Type NewState = EWeaponState::Idle;

	if (bPendingUnEquip)
	{
		NewState = EWeaponState::Unequipping;
	}
	else if (bPendingEquip)
	{
		NewState = EWeaponState::Equipping;
	}
	else
	{
		//Idle
	}
	SetWeaponState(NewState);
}

void AWeapon::SetWeaponState(EWeaponState::Type NewState)
{
	const EWeaponState::Type PrevState = CurrentState;
	CurrentState = NewState;
}

USkeletalMeshComponent * AWeapon::GetWeaponMesh() const
{
	return (MyPawn != NULL && MyPawn->IsFirstPerson()) ? FirstPersonMesh : ThirdPersonMesh;
}

ACombatCommander * AWeapon::GetPawnOwner() const
{
	return(MyPawn);
}

EWeaponState::Type AWeapon::GetCurrentState() const
{
	return CurrentState;
}

void AWeapon::OnRep_MyPawn()
{
	if (MyPawn)
	{
		OnEnterInventory(MyPawn);
	}
	else
	{
		OnLeaveInventory();
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, MyPawn);
}
