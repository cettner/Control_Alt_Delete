// Fill out your copyright notice in the Description page of Project Settings.

#include "PlacementActor.h"


// Sets default values
APlacementActor::APlacementActor() : Super()
{
	PlacementMeshComp = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("DefaultMesh"));
}

void APlacementActor::SetPrimitiveComp(UPrimitiveComponent* InMeshComp)
{
	PlacementMeshComp = InMeshComp;
}

UPrimitiveComponent* APlacementActor::GetPrimitive() const
{
	return PlacementMeshComp;
}
