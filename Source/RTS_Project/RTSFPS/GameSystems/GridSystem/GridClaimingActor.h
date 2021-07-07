// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridAttatchmentActor.h"
#include "SquareGameGrid.h"
#include "GridModifierType.h"

#include "Components/BoxComponent.h"
#include "GridClaimingActor.generated.h"


USTRUCT()
struct FGridTileOffset
{
	GENERATED_USTRUCT_BODY()

	int RowOffset = 0;
	int ColOffset = 0;

	friend bool operator == (const FGridTileOffset& Myself, const FGridTileOffset& Other)
	{
		bool isSame = Myself.RowOffset == Other.RowOffset;
		isSame &= Myself.ColOffset == Other.ColOffset;

		return(isSame);
	}
};

UCLASS()
class RTS_PROJECT_API AGridClaimingActor : public AGridAttatchmentActor
{
	GENERATED_BODY()

public:
	AGridClaimingActor();


protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginDestroy() override;

#if WITH_EDITOR
	virtual void OnConstruction(const FTransform & Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropChange) override;
#endif

protected:
	virtual void InitializeClaimSpace(ASquareGameGrid * InGrid);
	virtual void InitializeModifiers();
	virtual void PreTileChange(FGridTile NewTile);
	virtual void PostTileChange(FGridTile NewTile, FGridTile PrevTile = FGridTile());
	
public:
	virtual bool SetTileLocation(FGridTile Moveto) override;
protected:
	virtual ASquareGameGrid * AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid = nullptr) override;

	template <class T>
	T* GetActiveModifierOfClass() const;

public:
	virtual TArray<UGridModifierType *> GetActiveModifiers(FGridTile TileData);
	virtual TArray<FGridTileOffset> GetRelativeClaimSpace() const;
	virtual void SetGridClaimSpace(TArray<FGridTile> ClaimedTiles, ASquareGameGrid * OwningGrid);
	virtual TArray<FGridTile> GetGridClaimSpace() const;
	virtual void SetBoxExtent(FVector Extent);

public:
	virtual void SimulateModfiers();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Modifiers)
	UBoxComponent * ClaimSpaceComp;

protected:
    TArray<FGridTile> GridClaimSpace;
	TArray<FGridTileOffset> RelativeClaimSpace;

	UPROPERTY(EditDefaultsOnly, Category = Modifiers)
	TArray<TSubclassOf<UGridModifierType>> ModifierClasses;

	/*IMPORTANT: Array must be flagged as UPROPERTY or UObjects will be grabbed by garbage collector regardless of reference possession*/
	UPROPERTY()
	TArray<UGridModifierType *> Modifiers;


};

template<class T>
inline T* AGridClaimingActor::GetActiveModifierOfClass() const
{
	T* retval = nullptr;
	for (int i = 0; i < Modifiers.Num(); i++)
	{
		if (Cast<T>(Modifiers[i]) != nullptr)
		{
			retval = Cast<T>(Modifiers[i]);
			break;
		}
	}
	return retval;
}
