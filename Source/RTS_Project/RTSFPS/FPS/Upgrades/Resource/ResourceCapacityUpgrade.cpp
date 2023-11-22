// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceCapacityUpgrade.h"

#include "../../../GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"

bool UResourceCapacityUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
    bool retval = false;
    if (HasAuthority)
    {
        IResourceGatherer* gatherer = CastChecked<IResourceGatherer>(ToUpgrade);

        const uint32 currentmax = gatherer->GetResourceMaximum(TargetResource);
        const int32 rankdiff = static_cast<int32>(NewRank) - static_cast<int32>(OldRank);

        const int32 totalchange = rankdiff * BaseChangeValue;
        const int32 newmax = currentmax + totalchange;

        checkf(newmax > -1, TEXT("UResourceCapacityUpgrade::ApplyUpgrade calculated maximum was negative"))

        gatherer->SetResourceDiscreteMaximum(TargetResource, static_cast<uint32>(newmax));
        retval = true;
    }
    else
    {
        retval = true;
    }

    return retval;
}
