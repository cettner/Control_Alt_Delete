// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"







AResource::AResource() : Super() 
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCanEverAffectNavigation(true);
	MeshComp->bFillCollisionUnderneathForNavmesh = true;
	MeshComp->bReceivesDecals = false;

	SelectionComp = CreateDefaultSubobject<UDecalSelectionComponent>(TEXT("SelectionComp"));
	SelectionComp->SetDetection(MeshComp);
	SelectionComp->SetupAttachment(MeshComp);
	SetDeselected();

	UIData.ResourceName = "Default Resource";
	UIData.Key = GetClass();
}

uint32 AResource::Mine(uint32 AmountToMine)
{
	if ((ResourceVal > AmountToMine) || bisinfinite)
	{
		if (!bisinfinite)
		{
			ResourceVal -= AmountToMine;
		}

		return(AmountToMine);
	}
	else // theres not enough to give so give what's left.
	{
		uint32 retval = ResourceVal;
		ResourceVal = 0;
		Destroy();

		return(retval);
	}
}

FResourceUIData AResource::GetUIData() const
{
	return UIData;
}

uint32 AResource::GetResourceWeight() const
{
	return ResourceWeight;
}

bool AResource::CanAfford(FReplicationResourceMap BuyerResources, FReplicationResourceMap SellerCost)
{
	bool retval = true;
	const TMap<TSubclassOf<AResource>, int> buyermap = BuyerResources.GetMap();

	for (TPair<TSubclassOf<AResource>, int> Elem : SellerCost.GetMap())
	{
		const int32 * outval = buyermap.Find(Elem.Key);
		if (outval != nullptr && (retval == true))
		{
			/*Buyer has the resource, now check they have enough*/
			retval &= (*outval >= Elem.Value);
		}
		else
		{
			/*Buyer did not have the needed Resource or did not have enough*/
			retval = false;
			break;
		}
	}

	return retval;
}

void AResource::SetSelected()
{
	SelectionComp->SetHiddenInGame(false);
}

void AResource::SetDeselected()
{
	SelectionComp->SetHiddenInGame(true);
}



