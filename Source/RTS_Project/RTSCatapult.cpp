// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSCatapult.h"
#include "Runtime/Engine/Public/TimerManager.h "
#include "Runtime/Engine/Classes/Engine/World.h"
#include "ConstructorHelpers.h"
#include "Engine.h"

void ARTSCatapult::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(Launch_Handler, this, &ARTSCatapult::LaunchProjectile, 1.0, false, LaunchTime);

}

void ARTSCatapult::LaunchProjectile()
{
	UWorld* const World = GetWorld();
	ASiegeProjectile * SpawnedProjectile;
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		LaunchLocal = GetActorLocation();
		LaunchLocal.Z += 200;

		LaunchRot = GetActorRotation();
		LaunchRot.Pitch = 45;


		SpawnedProjectile = GetWorld()->SpawnActor<ASiegeProjectile>(Projectile,LaunchLocal,LaunchRot,SpawnParams);
	}




}

ARTSCatapult::ARTSCatapult()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileBlueprint(TEXT("Blueprint'/Game/TopDownBP/Actors/SiegeProjectile_BP.SiegeProjectile_BP'"));

	if (ProjectileBlueprint.Object)
	{
		Projectile = (UClass*)ProjectileBlueprint.Object->GeneratedClass;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Failed to Load Projectile Asset!")));
	}
}
