// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"

#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"





AResource::AResource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;


	
	UStaticMeshComponent* Mesh = GetStaticMeshComponent();
	
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCanEverAffectNavigation(true);
		Mesh->bFillCollisionUnderneathForNavmesh = true;
		Mesh->bReceivesDecals = false;
	}

	SelectionComp = CreateDefaultSubobject<UDecalSelectionComponent>(TEXT("SelectionComp"));
	SelectionComp->SetDetection(Mesh);
	SelectionComp->SetupAttachment(Mesh);
	SetDeselected();

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
	SelectionComp->SetHiddenInGame(false);
}

void AResource::SetDeselected()
{
	SelectionComp->SetHiddenInGame(true);
}



