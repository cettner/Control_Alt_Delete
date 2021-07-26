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

	void SetMeshComp(UMeshComponent * InMeshComp);
	UMeshComponent * GetMesh() const;

	template < class T >
	T* GetPrimitive() const
	{
		return Cast<T>(GetMesh());
	}

public:
	virtual void PreInitializePlacementActor(const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform = FTransform());
	virtual bool IsPlaceable() const;
	virtual void SetMeshColor(FLinearColor InColor);
	virtual TSubclassOf<AActor> GetEmulatedClass() const;

protected:
	UActorComponent* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass) const;


protected:
	UMeshComponent * MeshComp = nullptr;

	TSubclassOf<AActor> EmulatedClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Placement")
	UMaterial * PlacementMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Placement")
	FName ColorParameterName = "Color";

	UPROPERTY(EditDefaultsOnly, Category = "Placement")
	FName OpacityParameterName = "Opacity";

private:
	FLinearColor ActiveMeshColor = FLinearColor::Black;
};
