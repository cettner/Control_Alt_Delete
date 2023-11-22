// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceRegenUpgrade.h"



bool UResourceRegenUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
    bool retval = false;
    if (HasAuthority)
    {
        IResourceGatherer* gatherer = CastChecked<IResourceGatherer>(ToUpgrade);
        bool bShouldClearEvent = false;
        FResourceRegenEventConfig config = BuildConfigFromRank(gatherer, OldRank, NewRank, bShouldClearEvent);
        
        if (bShouldClearEvent)
        {
            gatherer->ClearResourceRegenEvent(TargetResource);
        }
        else
        {
            gatherer->AddResourceRegenEvent(config, TargetResource);
        }
        retval = true;
    }
    else
    {
        retval = true;
    }

	return retval;
}

FResourceRegenEventConfig UResourceRegenUpgrade::BuildConfigFromRank(const IResourceGatherer* InGatherer, const uint32 OldRank, const uint32 NewRank, bool& OutShouldClearEvent) const
{
    FResourceRegenEventConfig retval = FResourceRegenEventConfig();

    if (const FResourceRegenEventConfig * currentconfig = InGatherer->GetCurrentRegenEventConfig(TargetResource))
    {
        const int32 currentgain = currentconfig->FlatRegenAmount;
        const float currentrate = currentconfig->TickRate;

        if ((currentgain == DefaultRegenConfig.FlatRegenAmount) && (NewRank == UPGRADE_UNLEARNED))
        {
            OutShouldClearEvent = true;
        }
        else
        {
            OutShouldClearEvent = false;
            const int32 rankdiff = static_cast<int32>(NewRank) - static_cast<int32>(OldRank);

            const int32 totalgainchange = rankdiff * GainBasePerRank;
            const float totalratechange = static_cast<float>(rankdiff) * RateBasePerRank;

            const int32 newgain = currentgain + totalgainchange;
            const float newrate = currentrate + totalratechange;

            retval.FlatRegenAmount = newgain;
            retval.TickRate = newrate;
        }
    }
    else
    {
        retval = DefaultRegenConfig;
    }

    return retval;
}
