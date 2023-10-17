// Fill out your copyright notice in the Description page of Project Settings.


#include "ConjurableShield.h"

#include "Engine/DamageEvents.h"

AConjurableShield::AConjurableShield() : Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
	ShieldMesh->SetupAttachment(RootComponent);
}

void AConjurableShield::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	const TArray<UMaterialInterface *> materials = ShieldMesh->GetMaterials();
	if (materials.Num())
	{
		ShieldMaterial = UMaterialInstanceDynamic::Create(materials[0], this);
		ShieldMaterial->SetScalarParameterValue("Dissolve", -1.0f);
		ShieldMesh->SetMaterial(0, ShieldMaterial);
	}

}

float AConjurableShield::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FDamageEvent convertedevent = DamageEvent;
	convertedevent.DamageTypeClass = DamageConversionType;

	AActor* conjureactor = Cast<AActor>(PrimaryConjurer);
	const float retval = conjureactor->TakeDamage(Damage, convertedevent, EventInstigator, DamageCauser);

	return retval;
}

void AConjurableShield::ProcessConjureState()
{
	Super::ProcessConjureState();

	if (ConjureTimeToComplete != 0.0f)
	{
		const float conjurepercent = CurrentConjureTime  / ConjureTimeToComplete;
		ShieldMaterial->SetScalarParameterValue("Dissolve", conjurepercent);
		ShieldMesh->SetMaterial(0, ShieldMaterial);
	}
	else
	{
		ShieldMaterial->SetScalarParameterValue("Dissolve", 1.0f);
		ShieldMesh->SetMaterial(0, ShieldMaterial);
	}
}
