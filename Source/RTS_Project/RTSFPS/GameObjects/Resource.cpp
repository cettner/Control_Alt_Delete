// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"

#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"





AResource::AResource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

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
}

int AResource::Mine(uint32 amount_to_mine)
{
	if (resource_val > (int)amount_to_mine)
	{
		if (!bisinfinite)
		{
			resource_val -= amount_to_mine;
		}

		return(amount_to_mine);
	}
	else // theres not enough to give so give what's left.
	{
		int retval = resource_val;
		resource_val = 0;
		Destroy();

		return(retval);
	}
}

void AResource::SetSelected()
{
	Selection->SetSelected();
}

void AResource::SetDeselected()
{
	Selection->SetDeselected();
}



