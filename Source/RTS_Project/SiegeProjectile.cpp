// Fill out your copyright notice in the Description page of Project Settings.
#include "SiegeProjectile.h"
#include "Components/SphereComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"




// Sets default values
ASiegeProjectile::ASiegeProjectile(const FObjectInitializer& ObjectInitializer )
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionSphere->InitSphereRadius(Collision_Radius);
	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ASiegeProjectile::BeginPlay()
{
	Super::BeginPlay();
	Mesh->bUseDefaultCollision = false;
	Mesh->SetSimulatePhysics(true);
}



