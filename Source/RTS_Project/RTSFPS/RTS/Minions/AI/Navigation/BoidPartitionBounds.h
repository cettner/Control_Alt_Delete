// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BoidBoundsComponent.h"
#include "BoidPartitionBounds.generated.h"

UCLASS()
class RTS_PROJECT_API ABoidPartitionBounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoidPartitionBounds();
	UBoxPartitionComponent* GetPartitionFromPosition(const FVector& InPosition) const;


protected:
	UPROPERTY(EditDefaultsOnly)
	UBoidBoundsComponent* BoidBounds = nullptr;
};
