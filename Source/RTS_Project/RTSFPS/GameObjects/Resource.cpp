// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"

#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"





AResource::AResource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	Selection = CreateDefaultSubobject<URTSSelectionComponent>(TEXT("SelectionComp"));
	
	UStaticMeshComponent* Mesh = GetStaticMeshComponent();
	
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCanEverAffectNavigation(true);
		Mesh->bFillCollisionUnderneathForNavmesh = true;
		Selection->SetDetection(Mesh);
		Selection->SetRoot(Mesh);
	}

	UIData.ResourceName = "Default Resource";
	UIData.Key = GetClass();
}

int AResource::Mine(uint32 AmountToMine)
{
	if ((ResourceVal > (int)AmountToMine) || bisinfinite)
	{
		if (!bisinfinite)
		{
			ResourceVal -= AmountToMine;
		}

		return(AmountToMine);
	}
	else // theres not enough to give so give what's left.
	{
		int retval = ResourceVal;
		ResourceVal = 0;
		Destroy();

		return(retval);
	}
}

FResourceUIData AResource::GetUIData() const
{
	return UIData;
}

void AResource::SetSelected()
{
	Selection->SetSelected();
}

void AResource::SetDeselected()
{
	Selection->SetDeselected();
}



