// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"


// Sets default values
AWeapon::AWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	CollisionComp =  CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = CollisionComp;

	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}


Weapon_Grip_Type AWeapon::GetType()
{
	return(Grip_Type);
}

void AWeapon::Equipped(USkeletalMeshComponent * Character, FName Socketname)
{
	RootComponent->SetupAttachment(Character,Socketname);
}

void AWeapon::UnEquipped()
{
	RootComponent->DetachFromParent();
	Destroy();
}

