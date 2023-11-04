// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeManagerComponent.h"

// Sets default values for this component's properties
UUpgradeManagerComponent::UUpgradeManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UUpgradeManagerComponent::AddUpgrade(const TSubclassOf<UUpgrade>& InUpgrade)
{

}

void UUpgradeManagerComponent::RemoveUpgrade(const TSubclassOf<UUpgrade>& InUpgrade)
{

}

bool UUpgradeManagerComponent::RegisterUpgradable(IUpgradableInterface* InUpgradeObject)
{
	return false;
}

bool UUpgradeManagerComponent::UnRegisterUpgradable(IUpgradableInterface* InUpgradeObject)
{
	return false;
}

void UUpgradeManagerComponent::OnRep_AppliedUpgrades()
{
}

void UUpgradeManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

