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
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASiegeProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->IgnoreActorWhenMoving(Owner, true);
	Mesh->bUseDefaultCollision = false;
	Mesh->SetSimulatePhysics(true);
	Mesh->AddImpulse(FVector(9000.0,0, 9000.0));

}



