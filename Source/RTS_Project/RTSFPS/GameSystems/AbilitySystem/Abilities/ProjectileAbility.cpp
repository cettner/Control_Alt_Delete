 // Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbility.h"
#include "../AbilityComponent.h"

void UProjectileAbility::OnEffect()
{
	FTransform spawntransform = AbilityComp->GetCrosshairTransform(EffectSocketName);
	AAbilityProjectile * Projectile = AbilityComp->SpawnUninitializedActor<AAbilityProjectile>(ProjectileClass, spawntransform);
	Projectile->SetOwner(AbilityComp->GetOwner());
	Projectile->InitialSpeed = ProjectileSpeed;
	Projectile->CollisionChannel = AbilityEffectChannel;
	Projectile->SetIgnoredActors(AbilityComp->GetAbilityUser()->GetIgnoredTraceActors());

	AbilityComp->FinishSpawningActor(Projectile, spawntransform);
}
