// Fill out your copyright notice in the Description page of Project Settings.


#include "MineResourceAbility.h"

void UMineResourceAbility::ProcessTarget(AActor * Target)
{
    Super::ProcessTarget(Target);

}

bool UMineResourceAbility::CanHit(AActor * HitActor)
{
    const bool retval = Cast<AResource>(HitActor) != nullptr;
    return(retval);
}