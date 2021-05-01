// Fill out your copyright notice in the Description page of Project Settings.


#include "GridAttatchmentActor.h"

#include "EngineUtils.h"

// Sets default values
AGridAttatchmentActor::AGridAttatchmentActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SnapToComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Actor"));
	if (SnapToComp)
	{
		SnapToComp->SetupAttachment(RootComponent);
	}

}

void AGridAttatchmentActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	

}

void AGridAttatchmentActor::OnConstruction(const FTransform & Transform)
{
	if (ParentGrid)
	{
		UpdatePrimatives();
	}
	else
	{
		AttachToGrid(GetActorLocation());
		
		if (!bSavedOffsets)
		{
			/*Save the Original Local Offsets for all Prmitive Components*/
			SavePrimitiveOffsets();
			bSavedOffsets = SavedLocalOffsets.Num() > 0;
		}

		UpdatePrimatives();
	}
}

bool AGridAttatchmentActor::AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid, bool snaproot)
{
	ASquareGameGrid * foundgrid = nullptr;

	if (InGrid)
	{
		FGridTile foundtile = InGrid->GetTileFromLocation(StartLocation);
		if (foundtile.IsValid)
		{
			SetActorLocation(foundtile.TileCenter);
		}
	}
	else
	{
		foundgrid = FindGrid();
		if (foundgrid != nullptr)
		{
			FGridTile foundtile = foundgrid->GetTileFromLocation(StartLocation);
			if (foundtile.IsValid)
			{
				SetActorLocation(foundtile.TileCenter);
				RootGridLocation = foundtile;
			}
			SetParentGrid(foundgrid);
		}
	}

	return (InGrid || foundgrid);
}

void AGridAttatchmentActor::UpdatePrimatives()
{
	if (GetParentGrid() == nullptr) return;

	for (TPair<UPrimitiveComponent *, FVector> Elem : SavedLocalOffsets)
	{
		if (Elem.Key != nullptr)
		{
			FVector AttemptedTileLocation = GetActorLocation() + Elem.Value;
			FGridTile foundtile = GetParentGrid()->GetTileFromLocation(AttemptedTileLocation);

			if (foundtile.IsValid)
			{
				Elem.Key->SetWorldLocation(foundtile.TileCenter);
				PrimativeTileLocations.Add(Elem.Key, foundtile);
			}
			else
			{
				Elem.Key->SetWorldLocation(GetActorLocation() + Elem.Value);
				PrimativeTileLocations.Add(Elem.Key, foundtile);
			}
		}
	}
}

bool AGridAttatchmentActor::SetTileLocation(FGridTile Moveto)
{
	if (ParentGrid == nullptr || !Moveto.IsValid) return false;

	FVector movelocation = GetActorLocation();
		
	bool success = ParentGrid->GetLocationFromTile(Moveto, movelocation);
	SetActorLocation(movelocation);

	UpdatePrimatives();
	return(success);
}

ASquareGameGrid * AGridAttatchmentActor::FindGrid()
{
	UWorld * World;

	for (int i = 0; i < GEngine->GetWorldContexts().Num(); i++)
	{
		World = GEngine->GetWorldContexts()[i].World();
		if (World == nullptr) continue;

		for (TActorIterator<ASquareGameGrid> ActorItr(World); ActorItr; ++ActorItr)
		{
			ASquareGameGrid * found = *ActorItr;
			return(found);
		}
	}



	return nullptr;
}

void AGridAttatchmentActor::SetParentGrid(ASquareGameGrid * InGrid)
{
	ParentGrid = InGrid;
}

ASquareGameGrid * AGridAttatchmentActor::GetParentGrid() const
{
	return ParentGrid;
}

FGridTile AGridAttatchmentActor::GetRootGridTile() const
{
	return RootGridLocation;
}

bool AGridAttatchmentActor::CanBePlacedAt(FGridTile TestTile) const
{
	if (!TestTile.IsValid) return false;

	bool alltilesvalid = true;

	for (TPair<UPrimitiveComponent *, FVector> Elem : SavedLocalOffsets)
	{
		if (Elem.Key != nullptr)
		{
			FVector AttemptedTileLocation = TestTile.TileCenter + Elem.Value;
			FGridTile foundtile = GetParentGrid()->GetTileFromLocation(AttemptedTileLocation);
			alltilesvalid |= foundtile.IsValid;
		}
	}

	return (alltilesvalid);
}

void AGridAttatchmentActor::SavePrimitiveOffsets()
{
	TArray<UPrimitiveComponent *> comps = TArray<UPrimitiveComponent *>();
	GetComponents< UPrimitiveComponent>(comps);

	for (int i = 0; i < comps.Num(); i++)
	{
		UPrimitiveComponent * primitive = comps[i];

		if (primitive != nullptr)
		{
			SavedLocalOffsets.Add(primitive, primitive->GetRelativeLocation());
		}

	}
}

void AGridAttatchmentActor::SetRootGridTile(FGridTile InTile)
{
	RootGridLocation = InTile;
}
