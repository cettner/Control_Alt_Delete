// Fill out your copyright notice in the Description page of Project Settings.
#include "SiegeProjectile.h"
#include "Components/SphereComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"



// Sets default values
ASiegeProjectile::ASiegeProjectile(const FObjectInitializer& ObjectInitializer )
{
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	CollisionSphere->InitSphereRadius(Collision_Radius);
	CollisionSphere->OnComponentHit.AddDynamic(this, &ASiegeProjectile::OnHit);

	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionSphere;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void ASiegeProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->IgnoreActorWhenMoving(Owner, true);
	Mesh->bUseDefaultCollision = true;	
}


void ASiegeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Hit Another Actor!")));

		//Destroy();
}


