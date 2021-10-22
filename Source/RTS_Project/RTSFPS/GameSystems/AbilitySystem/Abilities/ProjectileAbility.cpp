 // Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbility.h"
#include "../AbilityComponent.h"

void UProjectileAbility::OnEffect()
{
	FTransform spawntransform = AbilityComp->GetCrosshairTransform(EffectSocketName);
	AAbilityProjectile * Projectile = AbilityComp->SpawnUninitializedActor<AAbilityProjectile>(ProjectileClass, spawntransform);
	Projectile->CollisionChannel = AbilityEffectChannel;
	Projectile->SetIgnoredActors(AbilityComp->GetAbilityUser()->GetIgnoredTraceActors());
	Projectile->SetOwner(AbilityComp->GetOwner());
	AbilityComp->FinishSpawningActor(Projectile, spawntransform);
}
