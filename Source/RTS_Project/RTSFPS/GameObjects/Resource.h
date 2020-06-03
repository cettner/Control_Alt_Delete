// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSFPS/BaseClasses/RTSMinion.h"
#include "RTSFPS/BaseClasses/RTSHUD.h"
#include "RTSFPS/RTS/Minions/Builder/RTSBUILDER.h"
#include "GameFramework/Actor.h"
#include "RTSFPS/BaseClasses/RTSPlayerController.h"
#include "RTSFPS/RTS/Camera/RTSSelectable.h"
#include "Resource.generated.h"



#define NUM_SLOTS 4

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
	ARTSHUD * HudPtr;
public:	

private:
	Resource_Types mytype = TYPE_ONE;

};
