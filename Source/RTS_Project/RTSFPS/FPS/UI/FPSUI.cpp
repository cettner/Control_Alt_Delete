// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSUI.h"


UFPSUI::UFPSUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HealthBar = CreateDefaultSubobject<UProgressBar>(TEXT("Health Bar"));
}

bool UFPSUI::Initialize()
{
	bool success = Super::Initialize();
	if (success == false || HealthBar == nullptr) return(success);

	HealthBar->PercentDelegate.BindUFunction(this, "UpdateHealthPercent");
	HealthBar->SynchronizeProperties();
	return success;
}

void UFPSUI::UpdateHealthPercent()
{
	
}
