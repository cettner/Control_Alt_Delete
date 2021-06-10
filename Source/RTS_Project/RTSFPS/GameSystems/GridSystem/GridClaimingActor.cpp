// Fill out your copyright notice in the Description page of Project Settings.


#include "GridClaimingActor.h"
#include "ClaimableSquareGameGrid.h"

AGridClaimingActor::AGridClaimingActor() : Super()
{
	ClaimSpaceComp = CreateDefaultSubobject<UBoxComponent>(TEXT("ClaimSpaceHelper"));
	if (ClaimSpaceComp)
	{
		ClaimSpaceComp->SetCanEverAffectNavigation(false);
	}

	ModifierClasses.AddUnique(UGridModifierType::StaticClass());
}


void AGridClaimingActor::PostInitializeComponents()
{
	InitializeModifiers();
	Super::PostInitializeComponents();
	PostTileChange(GetRootGridTile());
}

void AGridClaimingActor::OnConstruction(const FTransform & Transform)
{
	InitializeModifiers();
	Super::OnConstruction(Transform);

	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(GetParentGrid());

	if (claimgrid)
	{
		claimgrid->SimulateGrid();
	}
}

void AGridClaimingActor::InitializeClaimSpace(ASquareGameGrid * InGrid)
{
	if (!InGrid || !ClaimSpaceComp) return;
	RelativeClaimSpace.Empty();

	FVector extent = ClaimSpaceComp->GetUnscaledBoxExtent();
	float halftile = InGrid->GetTileSize() / 2.0f;

	int verticaltiles = roundf((extent.X + halftile) / InGrid->GetTileSize()) - 1U;
	int horizontaltiles = roundf((extent.Y + halftile) / InGrid->GetTileSize()) - 1U;

	FGridTileOffset LoadTile = FGridTileOffset();

	for (int i = -verticaltiles; i <= verticaltiles; i++)
	{
		for (int j = -horizontaltiles; j <= horizontaltiles; j++)
		{
			LoadTile.ColOffset = j;
			LoadTile.RowOffset = i;
			RelativeClaimSpace.AddUnique(LoadTile);
		}
	}
}

void AGridClaimingActor::InitializeModifiers()
{
	Modifiers.Empty();
	FName Modname = "DefaultModifierName";

	for (int j = 0; j < ModifierClasses.Num(); j++)
	{
		UGridModifierType * newmod = NewObject<UGridModifierType>(ModifierClasses[j], Modname);
		if (newmod != nullptr)
		{
			Modifiers.Add(newmod);
		}
	}
}

TArray<UGridModifierType *> AGridClaimingActor::GetActiveModifiers(FGridTile TileData)
{
	return Modifiers;
}

void AGridClaimingActor::PreTileChange(FGridTile NewTile)
{
	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(GetParentGrid());
	if (claimgrid == nullptr || !NewTile.IsValid) return;

	for (int i = 0; i < GridClaimSpace.Num(); i++)
	{
		for (int j = 0; j < Modifiers.Num(); j++)
		{
			claimgrid->RemoveModifier(GridClaimSpace[i], Modifiers[j], this);
		}
	}


}

void AGridClaimingActor::PostTileChange(FGridTile NewTile, FGridTile PrevTile)
{
	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(GetParentGrid());
	if (claimgrid == nullptr) return;

		for (int j = 0; j < Modifiers.Num(); j++)
		{
			 claimgrid->ApplyModifier(GridClaimSpace, Modifiers[j], this);
		}
}

ASquareGameGrid * AGridClaimingActor::AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid)
{
	ASquareGameGrid * foundgrid = Super::AttachToGrid(StartLocation, InGrid);
	InitializeClaimSpace(foundgrid);

	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(foundgrid);

	if (claimgrid && claimgrid->AddGridActor(this, GetRootGridTile()))
	{
		//PostTileChange(GetRootGridTile());
	}


	return(claimgrid);
}

bool AGridClaimingActor::SetTileLocation(FGridTile Moveto)
{
	bool success = false;
	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(GetParentGrid());
	if (claimgrid)
	{
		FGridTile prev = GetRootGridTile();
		PreTileChange(Moveto);
		success = claimgrid->MoveGridActor(this, Moveto);
		PostTileChange(Moveto, prev);
	}


	return success;
}

TArray<FGridTileOffset> AGridClaimingActor::GetRelativeClaimSpace() const
{
	return RelativeClaimSpace;
}

void AGridClaimingActor::SetGridClaimSpace(TArray<FGridTile> ClaimedTiles, ASquareGameGrid * OwningGrid)
{
	GridClaimSpace = ClaimedTiles;
}

TArray<FGridTile> AGridClaimingActor::GetGridClaimSpace() const
{
	return GridClaimSpace;
}

void AGridClaimingActor::SimulateModfiers()
{
	InitializeModifiers();

	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(GetParentGrid());
	for (int j = 0; j < Modifiers.Num(); j++)
	{
		claimgrid->ApplyModifier(GridClaimSpace, Modifiers[j], this);
	}

	Modifiers.Empty();
}
