// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityExplosion.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAbilityExplosion::AAbilityExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	//Definition for the SphereComponent that will serve as the Root component for the projectile and its collision.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(5.5f);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	RootComponent = SphereComponent;
}

void AAbilityExplosion::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BaseParticleEffect, GetTransform());
	SetLifeSpan(2.0f);
}