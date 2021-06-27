// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridClaimingActor.h"


#include "PlacementActor.generated.h"

UCLASS()
class RTS_PROJECT_API APlacementActor : public AGridClaimingActor
{
	GENERATED_BODY()
	
public:	
	APlacementActor();

	void SetPrimitiveComp(UPrimitiveComponent * InMeshComp);
	UPrimitiveComponent * GetPrimitive() const;

	template < class T >
	T* GetPrimitive() const
	{
		return Cast<T>(GetPrimitive());
	}

protected:
	UPrimitiveComponent * PlacementMeshComp = nullptr;
	
};
