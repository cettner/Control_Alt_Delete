// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"


bool UHealthUpgrade::CanUpgrade(const IUpgradableInterface * TestUpgrade) const
{
	bool retval = Super::CanUpgrade(TestUpgrade);

	if (retval == true)
	{
		const bool hashealthcomp = TryGetHealthComp(TestUpgrade) != nullptr;
		retval &= hashealthcomp;
	}

	return retval;
}

UHealthComponent * UHealthUpgrade::TryGetHealthComp(const IUpgradableInterface * TestUpgrade) const
{
	UHealthComponent * retval = nullptr;
	const AActor * actorcheck = TestUpgrade->GetUpgradeApplicationClass()->GetDefaultObject<AActor>();
	
	if (actorcheck != nullptr)
	{
		retval = Cast<UHealthComponent>(actorcheck->GetComponentByClass(UHealthComponent::StaticClass()));
	}
	return retval;
}

UHealthComponent * UHealthUpgrade::TryGetHealthComp(const UObject * TestUpgrade) const
{
	UHealthComponent * retval = nullptr;
	const AActor * actorcheck = Cast<AActor>(TestUpgrade);

	if (actorcheck != nullptr)
	{
		retval = Cast<UHealthComponent>(actorcheck->GetComponentByClass(UHealthComponent::StaticClass()));
	}
	return retval;
}

void UHealthUpgrade::ApplyUpgrade(UObject * ToUpgrade) const
{
	UHealthComponent * healthcomp = TryGetHealthComp(ToUpgrade);
	const float currentmaxheath = healthcomp->GetMaxHealth();
	const float newmaxhealth = currentmaxheath + MaxHealthIncrement;
	healthcomp->SetMaxHealth(newmaxhealth);
}
