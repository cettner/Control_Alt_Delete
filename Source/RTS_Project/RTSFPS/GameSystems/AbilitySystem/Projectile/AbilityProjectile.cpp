// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityProjectile.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"

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
	SphereComponent->InitSphereRadius(5.5f);
	SphereComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	if (HasAuthority())
	{
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	}
	else
	{
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

	RootComponent = SphereComponent;

	FlightParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	FlightParticles->SetupAttachment(RootComponent);

}

void AAbilityProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (HasAuthority())
	{
		SphereComponent->SetCollisionResponseToChannel(CollisionChannel, ECollisionResponse::ECR_Block);
		SphereComponent->OnComponentHit.AddDynamic(this, &AAbilityProjectile::OnProjectileImpact);

		SetLifeSpan(ProjectileLifeTime);
		OnRep_InitialSpeed();
	}
}

// Called every frame
void AAbilityProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbilityProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (OnHitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OnHitParticles, GetTransform());
	}
	Super::EndPlay(EndPlayReason);
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
	IgnoredActors.AddUnique(IgnoreThis);
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

		Destroy();
	}
}

void AAbilityProjectile::OnRep_InitialSpeed()
{
	FVector fv = GetActorForwardVector();
	check(MovementComp);
	MovementComp->Velocity = InitialSpeed * fv;
}


void AAbilityProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AAbilityProjectile, InitialSpeed, COND_InitialOnly);
}