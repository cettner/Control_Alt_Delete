// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSMinion.h"
#include "RTSHUD.h"
#include "RTSBUILDER.h"
#include "GameFramework/Actor.h"
#include "Resource.generated.h"


UCLASS(Blueprintable)
class RTS_PROJECT_API AResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource(const FObjectInitializer& ObjectInitializer);
	int Mine(UINT amount_to_mine);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Category = Default)
	void OnRightClick(AActor* Target, FKey ButtonPressed);

	TArray <ARTSMinion*> Tasked_Units;
	TArray <ARTSBUILDER*> Mining_Units;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool is_infinite;
	UINT resource_val = 500;

	ARTSHUD * HudPtr;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StaticMeshComponents)
	UStaticMeshComponent* Mesh;

private:
	

	
};
