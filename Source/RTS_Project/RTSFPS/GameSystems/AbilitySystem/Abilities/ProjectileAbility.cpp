 // Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbility.h"
#include "../AbilityComponent.h"

void UProjectileAbility::OnEffect()
{
	FTransform spawntransform = AbilityComp->GetCrosshairTransform(EffectSocketName);
	AActor* Projectile = AbilityComp->SpawnUninitializedActor(ProjectileClass, spawntransform);
	AbilityComp->FinishSpawningActor(Projectile, spawntransform);
}
