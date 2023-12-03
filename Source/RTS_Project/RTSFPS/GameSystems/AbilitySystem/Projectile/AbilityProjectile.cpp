// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityProjectile.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAbilityProjectile::AAbilityProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	MovementComp->ProjectileGravityScale = 0.0f;

	//Definition for the SphereComponent that will serve as the Root component for the projectile and its collision.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(2.5f);
	SphereComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	RootComponent = SphereComponent;

	FlightParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	FlightParticles->SetupAttachment(RootComponent);
}

void AAbilityProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComponent->SetCollisionObjectType(CollisionChannel);
	if (HasAuthority())
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &AAbilityProjectile::OnProjectileImpact);
		SetLifeSpan(ProjectileLifeTime);
		OnRep_InitialSpeed();
	}
}

void AAbilityProjectile::OnRep_Owner()
{
	IAbilityUserInterface * owner = GetOwner<IAbilityUserInterface>();
	if (owner)
	{
		SetIgnoredActors(owner->GetIgnoredTraceActors());
	}
}

void AAbilityProjectile::SetIgnoredActors(TArray<AActor*> IgnoreThese)
{
	for (int i = 0; i < IgnoreThese.Num(); i++)
	{
		SetIgnoredActor(IgnoreThese[i]);
	}
}

void AAbilityProjectile::SetIgnoredActor(AActor * IgnoreThis)
{
	SphereComponent->IgnoreActorWhenMoving(IgnoreThis,true);
	IgnoredActors.Emplace(IgnoreThis);
}

AAbilityExplosion* AAbilityProjectile::SpawnExplosionatLocation(const FTransform& InTransform) const
{
	UWorld* world = GetWorld();
	FActorSpawnParameters spawnparams;
	spawnparams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnparams.Owner = GetOwner();

	AAbilityExplosion* explosion = world->SpawnActor<AAbilityExplosion>(ExplosionClass, InTransform, spawnparams);

	return explosion;
}

void AAbilityProjectile::SetHasDetonated(const bool InDetonated)
{
	if (HasAuthority())
	{
		bHasDetonated = InDetonated;
		if (GetNetMode() != ENetMode::NM_DedicatedServer)
		{
			OnRep_bHasDetonated();
		}
	}
}

void AAbilityProjectile::OnProjectileImpact(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor && !IgnoredActors.Contains(OtherActor))
	{
		if (OtherActor->CanBeDamaged())
		{
			FDamageEvent ondamageevent = FDamageEvent();
			OtherActor->TakeDamage(DirectDamage, ondamageevent, nullptr, this);
		}

		if (bDetonatesOnImpact == true)
		{
			OnDetonation(Hit.Location, Hit.Normal.Rotation(), GetActorForwardVector());
		}

	}
}

void AAbilityProjectile::OnDetonation(const FVector& InLocation, const FRotator& HitNormal, const FVector& ProjectileNormal)
{	
	if (HasAuthority())
	{
		SetHasDetonated(true);
		
		const FTransform explosiontransform = FTransform(HitNormal, InLocation, FVector(1.0,1.0,1.0));

		SpawnExplosionatLocation(explosiontransform);
		SetLifeSpan(2.0f);
	}


}

void AAbilityProjectile::OnRep_InitialSpeed()
{
	const FVector fv = GetActorForwardVector();
	MovementComp->Velocity = InitialSpeed * fv;
}

void AAbilityProjectile::OnRep_bHasDetonated()
{
	if (bHasDetonated == true)
	{
		if (IsValid(FlightParticles))
		{
			FlightParticles->Deactivate();
		}
	}
}

void AAbilityProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AAbilityProjectile, InitialSpeed, COND_InitialOnly);
	DOREPLIFETIME(AAbilityProjectile, bHasDetonated);
}

void AAbilityProjectile::LifeSpanExpired()
{
	if (HasAuthority() && !bHasDetonated && bDetonatesOnLifeTime)
	{
		SpawnExplosionatLocation(GetActorTransform());
	}

	Super::LifeSpanExpired();
}
