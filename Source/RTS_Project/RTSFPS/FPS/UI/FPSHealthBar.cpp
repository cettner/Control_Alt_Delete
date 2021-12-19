// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHealthBar.h"

void UFPSHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	PercentDelegate.BindUFunction(this, "UpdatePercentHealth");
}

float UFPSHealthBar::UpdatePercentHealth()
{
	float percenthealth = 0.0f;

	UHealthComponent* health = GetOwnerHealthComp();
	if (health == nullptr) return(percenthealth);


	percenthealth = health->GetHealthPercentage();

	return(percenthealth);
}


UHealthComponent* UFPSHealthBar::GetOwnerHealthComp() const
{
	UHealthComponent* health = nullptr;

	TArray<UHealthComponent*> Healthcomps;
	APawn* MyPawn = GetOwningPlayerPawn();

	if (!MyPawn || MyPawn->IsPendingKill())
	{
		return(health);
	}

	MyPawn->GetComponents<UHealthComponent>(Healthcomps);
	if (Healthcomps.Num() > 0)
	{
		health = Healthcomps[0];
	}
	return health;
}