// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrderManager.h"

URTSOrderGroup* ARTSOrderManager::BuildOrderGroup(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const FHitResult& InHitContext, AController* Issuer)
{
    URTSOrderGroup* group = NewObject<URTSOrderGroup>(this);
    const uint32 nextid = GenerateNextGroupID();
    checkf(group->InitalizeOrderGroup(nextid, InUnits, Issuer, InHitContext), TEXT("ARTSOrderManager::BuildOrderGroup Failed To Initialize group for all group members"));
    OrderGroups.Emplace(nextid, group);
    group->IssueAllOrders();
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Group Created")));

    return group;
}

URTSOrderGroup* ARTSOrderManager::GetGroupByID(const uint32 InID) const
{
    URTSOrderGroup* retval = nullptr;
    if (URTSOrderGroup* const* foundgroup = OrderGroups.Find(InID))
    {
        retval = *foundgroup;
    }

    return retval;
}

void ARTSOrderManager::OnGroupEmptied(URTSOrderGroup* EmptyGroup)
{
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Group Destroyed")));
    OrderGroups.Remove(EmptyGroup->GetOrderID());
    EmptyGroup->ConditionalBeginDestroy();
}
