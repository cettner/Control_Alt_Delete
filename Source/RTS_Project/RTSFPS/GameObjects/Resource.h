// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../RTS/Camera/RTSSelectable.h"
#include "Resource.generated.h"



#define NUM_SLOTS 4

enum Resource_Types
{
	TYPE_ONE,
	TYPE_TWO,
	TYPE_THREE,
	NULL_TYPE
};


UCLASS(Blueprintable)
class RTS_PROJECT_API AResource : public ARTSSelectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource(const FObjectInitializer& ObjectInitializer);
	int Mine(UINT amount_to_mine, Resource_Types& type);

	FVector GetSlot(int &ref_idx);
	void FreeSlot(int i);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray <bool> Slot_Available;
	TArray <FVector> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool is_infinite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int resource_val = 90;

	virtual void SetType(Resource_Types typeset);

private:
	Resource_Types mytype = TYPE_ONE;

};
