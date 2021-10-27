// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeManager.h"

AUpgradeManager::AUpgradeManager() : Super()
{
	bReplicates = true;
}

bool AUpgradeManager::DispatchUpgrade_Validate(TSubclassOf<UUpgrade> UpgradeClass, const TArray<AActor*> & Applyto)
{
	return true;
}

void AUpgradeManager::DispatchUpgrade_Implementation(TSubclassOf<UUpgrade> UpgradeClass, const TArray<AActor*> & Applyto)
{
	if (HasAuthority())
	{
		UUpgrade * newupgrade = NewObject<UUpgrade>(UUpgrade::StaticClass(), UpgradeClass);
		for (int i = 0; i < Applyto.Num(); i++)
		{
			IUpgradableInterface * UpgradeMe = Cast<IUpgradableInterface>(Applyto[i]);
			UpgradeMe->OnApplyUpgrade(newupgrade);
		}
	}


}

bool AUpgradeManager::CheckAndDispatchUpgrade(TSubclassOf<UUpgrade> UpgradeClass, TArray<AActor*>& Applyto)
{ 
	if (!HasAuthority()) return false;

	for (int i = 0; i < Applyto.Num(); i++)
	{
		IUpgradableInterface * UpgradeMe = Cast<IUpgradableInterface>(Applyto[i]);
		if (UpgradeMe == nullptr)
		{
			Applyto.RemoveAt(i);
			UE_LOG(LogTemp, Warning, TEXT("WARNING AUpgradeManager::CheckAndDispatchUpgrade Actor Does not implement Upgradable interface, Removing it from RPC"));
		}
	}

	DispatchUpgrade(UpgradeClass, Applyto);

	return true;
}
