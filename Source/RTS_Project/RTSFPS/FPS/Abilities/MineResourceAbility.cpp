// Fill out your copyright notice in the Description page of Project Settings.


#include "MineResourceAbility.h"
#include "RTS_Project/RTSFPS/GameObjects/Resource.h"
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

void UMineResourceAbility::MineResource()
{
	AResource * nodetomine = AbilityComp->GetAbilityTarget<AResource>();
	IResourceGatherer * resourcegatherer = GetResourceGatherer();

	if (IsValid(nodetomine) && (resourcegatherer != nullptr))
	{
		TSubclassOf<AResource> resourceclass = nodetomine->GetClass();
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
		GetWorld()->GetTimerManager().SetTimer(MineTickHandler, this, &UMineResourceAbility::MineResource, MiningRate, false);
	}
}

void UMineResourceAbility::SeverBeam()
{
	Super::SeverBeam();
	GetWorld()->GetTimerManager().ClearTimer(MineTickHandler);
}

bool UMineResourceAbility::CanHit(AActor * HitActor) const
{
    const bool retval = Cast<AResource>(HitActor) != nullptr;
    return(retval);
}

