// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/SkinnedMeshComponent.h"
#include "UnrealNetwork.h"
#include "CombatCommander.h"
#include "Components/SkeletalMeshComponent.h"



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
}

void AWeapon::OnEquipFinished()
{
}

void AWeapon::OnUnEquip()
{
}

void AWeapon::OnEnterInventory(ACombatCommander * NewOwner)
{
}

void AWeapon::OnLeaveInventory()
{
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

USkeletalMeshComponent * AWeapon::GetWeaponMesh() const
{
	return (MyPawn != NULL && MyPawn->IsFirstPerson()) ? FirstPersonMesh : ThirdPersonMesh;
}

ACombatCommander * AWeapon::GetPawnOwner() const
{
	return(MyPawn);
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

void AWeapon::Equipped(USkeletalMeshComponent * Character, FName Socketname)
{
	RootComponent->SetupAttachment(Character,Socketname);
}

void AWeapon::UnEquipped()
{
	//Calls Modify Function on Root Component if set tot true
	bool bInCallModify = false;
	FDetachmentTransformRules detach(EDetachmentRule::KeepRelative, bInCallModify);
	RootComponent->DetachFromComponent(detach);
	Destroy();
}


void AWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon, MyPawn);
}
