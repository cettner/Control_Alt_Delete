// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/GridClaimingActor.h"


#include "Components/StaticMeshComponent.h"
#include "Resource.generated.h"

USTRUCT(Blueprintable)
struct FResourceUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName ResourceName = "";

	UPROPERTY(EditDefaultsOnly)
	UTexture* ResourceImage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AResource> Key = nullptr;
};

UCLASS(Blueprintable)
class RTS_PROJECT_API AResource : public AGridClaimingActor, public IRTSObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResource();
	int Mine(uint32 amount_to_mine);
	FResourceUIData GetUIData() const;

public:
	/*RTSObject Interface Overrides*/
	virtual void SetSelected() override;
	virtual void SetDeselected() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	UDecalSelectionComponent* SelectionComp;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	UStaticMeshComponent* MeshComp;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bisinfinite = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int ResourceVal = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FResourceUIData UIData;
};
