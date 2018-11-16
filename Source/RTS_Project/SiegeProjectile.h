// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SiegeProjectile.generated.h"

UCLASS()
class RTS_PROJECT_API ASiegeProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASiegeProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	float angle_init = 0;
	float init_velocity = 0;
	AActor * Owner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Collision")
		float Collision_Radius = 200.0;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
		class USphereComponent* CollisionSphere;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = StaticMeshComponents)
		UStaticMeshComponent * Mesh;
};
