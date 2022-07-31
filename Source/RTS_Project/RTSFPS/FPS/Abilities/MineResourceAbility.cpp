// Fill out your copyright notice in the Description page of Project Settings.


#include "MineResourceAbility.h"
#include "RTS_Project/RTSFPS/GameObjects/Resources/Resource.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/AbilityComponent.h"


UMineResourceAbility::UMineResourceAbility() : Super()
{
	bLinkBeamToTarget = true;
}

float UMineResourceAbility::GetMiningRate() const
{
	return MiningRate;
}

void UMineResourceAbility::SetMiningRate(float InRate)
{
	MiningRate = InRate;
}

uint32 UMineResourceAbility::GetMineAmount() const
{
	return MineAmount;
}

void UMineResourceAbility::SetMineAmount(uint32 InAmount)
{
	MineAmount = InAmount;
}

void UMineResourceAbility::MineResource()
{
	AResource * nodetomine = AbilityComp->GetAbilityTarget<AResource>();
	IResourceGatherer * resourcegatherer = GetResourceGatherer();
	checkf(resourcegatherer, TEXT("UMineResourceAbility::MineResource failed to obtain ResourceGatherer"));

	if (IsValid(nodetomine))
	{
		const TSubclassOf<AResource> resourceclass = nodetomine->GetClass();
		const uint32 maxpull = resourcegatherer->CanCarryMore(resourceclass);
		
		if (maxpull > 0U)
		{
			uint32 actualpull = maxpull;
			if (maxpull > MineAmount)
			{
				actualpull = MineAmount;
			}
			const uint32 minedresource = nodetomine->Mine(actualpull);
			resourcegatherer->AddResource(resourceclass, actualpull);
		}

	}
}

IResourceGatherer * UMineResourceAbility::GetResourceGatherer() const
{

	return 	AbilityComp->GetOwner<IResourceGatherer>();;
}

void UMineResourceAbility::OnAbilityEnd()
{
	Super::OnAbilityEnd();
	GetWorld()->GetTimerManager().ClearTimer(MineTickHandler);
}

void UMineResourceAbility::StartBeam()
{
	Super::StartBeam();
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(MineTickHandler, this, &UMineResourceAbility::MineResource, MiningRate, true);
	}
}

void UMineResourceAbility::SeverBeam()
{
	Super::SeverBeam();
	GetWorld()->GetTimerManager().ClearTimer(MineTickHandler);
}

bool UMineResourceAbility::ShouldSeverBeam() const
{
	bool retval = Super::ShouldSeverBeam();
	if (retval == false)
	{
		const IResourceGatherer* resourcegatherer = GetResourceGatherer();
		checkf(resourcegatherer, TEXT("UMineResourceAbility::ShouldSeverBeam failed to obtain ResourceGatherer"));
		
		const AResource * nodetomine = AbilityComp->GetAbilityTarget<AResource>();
		retval |= !IsValid(nodetomine);

		if (retval == false)
		{
			const uint32 resourcecapacity = resourcegatherer->CanCarryMore(nodetomine->GetClass());
			retval |= (resourcecapacity == 0U);
		}
	}

	return retval;
}

bool UMineResourceAbility::CanHit(AActor * HitActor) const
{
    const bool retval = Cast<AResource>(HitActor) != nullptr;
    return(retval);
}

