// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BoidPartitionBounds.generated.h"

UCLASS()
class RTS_PROJECT_API ABoidPartitionBounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidPartitionBounds();

public:

protected:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* BoidBounds = nullptr;

	static void CalculateBoxCenters(const FVector& InBoxCenter, const FVector& InBoxExtent, const uint32 InNumLengthPartitions, const uint32 InNumWidthPartitions, const uint32 InNumHeightPartitons, TArray<FVector>& OutCenters, FVector& OutExtent);

};
