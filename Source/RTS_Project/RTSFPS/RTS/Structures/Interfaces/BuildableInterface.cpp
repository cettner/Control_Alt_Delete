// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildableInterface.h"

// Add default functionality here for any IBuildableInterface functions that are not pure virtual.


float IBuildableInterface::GetPercentConstructed() const
{
	return PercentConstructed;
}

bool IBuildableInterface::IsConstructed() const
{
	return bISConstructed;
}

void IBuildableInterface::BeginConstruction()
{

}

void IBuildableInterface::IncrementConstruction(float DeltaConstruction, AActor* Contributor)
{
	PercentConstructed += DeltaConstruction;
}

void IBuildableInterface::OnConstructionComplete()
{
	bISConstructed = true;
}
