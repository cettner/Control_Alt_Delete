// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeData.h"

void UUpgradeData::OnRep_UpgradeState(TArray<FUpgradeInfo> OldUpgrades)
{
}

void UUpgradeData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

bool UUpgradeData::IsSupportedForNetworking() const
{
	return true;
}
