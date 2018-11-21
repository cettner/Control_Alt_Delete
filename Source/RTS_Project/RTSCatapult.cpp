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

		FTransform object;

		object.SetTranslation(LaunchLocal);
		object.SetRotation(LaunchRot.Quaternion());

		SpawnedProjectile = GetWorld()->SpawnActorDeferred<ASiegeProjectile>(Projectile, object, nullptr, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		SpawnedProjectile->init_velocity = 20.0f;
		SpawnedProjectile->angle_init = 45.0f;
		SpawnedProjectile->Owner = this;


		UGameplayStatics::FinishSpawningActor(SpawnedProjectile,object);
		GetWorldTimerManager().SetTimer(Launch_Handler, this, &ARTSCatapult::LaunchProjectile, 1.0, false, LaunchTime);
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



	Min_Range_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("MinRange"));
	Min_Range_Sphere->InitSphereRadius(Min_Range);
	Min_Range_Sphere->AttachTo(RootComponent);
	Min_Range_Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARTSCatapult::OnMinRangeOverlapBegin);
	Min_Range_Sphere->OnComponentEndOverlap.AddDynamic(this,&ARTSCatapult::OnMinRangeOverlapEnd);

	Max_Range_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("MaxRange"));
	Max_Range_Sphere->InitSphereRadius(Max_Range);
	Max_Range_Sphere->AttachTo(RootComponent);
	Max_Range_Sphere->OnComponentBeginOverlap.AddDynamic(this, &ARTSCatapult::OnMaxRangeOverlapBegin);
	Max_Range_Sphere->OnComponentEndOverlap.AddDynamic(this, &ARTSCatapult::OnMaxRangeOverlapEnd);
}

void ARTSCatapult::OnMinRangeOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ARTSMinion>(OtherActor) && Cast<ARTSMinion>(OtherActor)->team_index != team_index)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("Enemy too close to fire!")));
	}

}

void ARTSCatapult::OnMinRangeOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ARTSMinion>(OtherActor) && Cast<ARTSMinion>(OtherActor)->team_index != team_index)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("Enemy in range!")));
	}
}

void ARTSCatapult::OnMaxRangeOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<ARTSMinion>(OtherActor) && Cast<ARTSMinion>(OtherActor)->team_index != team_index)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("Enemy in range!")));
	}
}

void ARTSCatapult::OnMaxRangeOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ARTSMinion>(OtherActor) && Cast<ARTSMinion>(OtherActor)->team_index != team_index)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta, FString::Printf(TEXT("Enemy has left max range!")));
	}
}
