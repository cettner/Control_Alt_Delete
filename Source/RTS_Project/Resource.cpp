// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"

#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"




AResource::AResource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AResource::SetType(Resource_Types typeset)
{
	mytype = typeset;
}

// Called when the game starts or when spawned
void AResource::BeginPlay()
{
	Super::BeginPlay();
	FVector currentlocal = GetActorLocation();
	FVector AddLocal = currentlocal;
	float radius = 120.0;

	AddLocal.X = currentlocal.X + radius;
	Slots.Add(AddLocal);

	AddLocal.X = currentlocal.X - radius;
	Slots.Add(AddLocal);

	AddLocal.X = currentlocal.X;

	AddLocal.Y = currentlocal.Y + radius;
	Slots.Add(AddLocal);

	AddLocal.Y = currentlocal.Y - radius;
	Slots.Add(AddLocal);

	for (int i = 0; i < Slots.Num(); i++)
	{
		Slot_Available.Add(true);
	}
}

int AResource::Mine(UINT amount_to_mine, Resource_Types& type )
{
	type = mytype;
	if (resource_val > (int)amount_to_mine)
	{
		resource_val -= amount_to_mine;
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

// attempts to get a valid node location, if unsuccesful, returns nodes rootlocation
FVector AResource::GetSlot(int &ref_idx)
{
	for (int i = 0; i < NUM_SLOTS; i++)
	{
		if (Slot_Available[i])
		{
			Slot_Available[i] = false;
			ref_idx = i;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor Claimed Slot %d"),i));
			return(Slots[i]);
		}		
	}	

	return(this->GetActorLocation());
}

void AResource::FreeSlot(int i)
{
	Slot_Available[i] = true;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Actor Freed Slot %d"), i));
}


