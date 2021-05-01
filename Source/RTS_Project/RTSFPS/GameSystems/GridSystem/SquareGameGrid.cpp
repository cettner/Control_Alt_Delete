// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareGameGrid.h"
#include "Materials/MaterialInstance.h"

#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ASquareGameGrid::ASquareGameGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	LinesProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GridMesh"));
	SelectionProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SelectionMesh"));

	LinesProceduralMesh->SetupAttachment(RootComponent);
	SelectionProceduralMesh->SetupAttachment(RootComponent);
}

void ASquareGameGrid::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASquareGameGrid::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	DrawGrid();
}

void ASquareGameGrid::SetSelectedTiles(TArray<FGridTile> SelectedTiles)
{
	TArray<FVector> selectedverts = TArray<FVector>();
	TArray<int> selectedtris = TArray<int>();

	for (int i = 0; i < SelectedTiles.Num(); i++)
	{
		FGridTile currenttile = SelectedTiles[i];
		if (IsTileValid(currenttile.row, currenttile.column))
		{
			float ystart = (TileSize / 2.0f) + (currenttile.column * TileSize);
			float xstart = currenttile.row * TileSize;
			float xend = TileSize + (TileSize * currenttile.row);
			float yend = (TileSize / 2.0f) + (currenttile.column * TileSize);

			FVector selectstart = FVector(xstart, ystart, 0.0f);
			FVector selectend = FVector(xend, yend, 0.0f);

			DrawLine(selectstart, selectend, TileSize, selectedverts, selectedtris);
		}
	}


	if (SelectionProceduralMesh && GridMaterial)
	{
		UMaterialInstanceDynamic * selectionmaterial = UMaterialInstanceDynamic::Create(GridMaterial, this);


		selectionmaterial->SetVectorParameterValue(ColorParameterName, SelectionColor);
		selectionmaterial->SetScalarParameterValue(OpacityParameterName, SelectionOpacity);

		SelectionProceduralMesh->CreateMeshSection(0, selectedverts, selectedtris, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
		SelectionProceduralMesh->SetMaterial(0, selectionmaterial);
	}
}

float ASquareGameGrid::GetGridWidth() const
{
	return NumColumns * TileSize;
}

float ASquareGameGrid::GetGridHieght() const
{
	return NumRows * TileSize;
}

float ASquareGameGrid::GetGridElevation(FGridTile Tiledata) const
{
	return GetActorLocation().Z;
}

bool ASquareGameGrid::IsTileValid(int Row, int Column) const
{
	if (Row >= 0 && Row < NumRows && Column >= 0 && Column < NumColumns)
	{
		return(true);
	}

	return(false);
}

FGridTile ASquareGameGrid::GetTileFromLocation(FVector Location) const
{
	FGridTile retval;
	FVector gridstartlocation = GetActorLocation();
	float approxcol = ((Location.Y - gridstartlocation.Y) / GetGridHieght()) * NumColumns;
	float approxrow = ((Location.X - gridstartlocation.X) / GetGridWidth()) * NumRows;
	float centeroffset = TileSize / 2.0f;

	retval.row = floorf(approxrow);
	retval.column = floorf(approxcol);
	retval.IsValid = IsTileValid(retval.row, retval.column);

	if (retval.IsValid)
	{
		retval.TileCenter.X = gridstartlocation.X + (retval.row * TileSize) + centeroffset;
		retval.TileCenter.Y = gridstartlocation.Y + (retval.column * TileSize) + centeroffset;
		retval.TileCenter.Z = GetGridElevation(retval);
	}


	return (retval);
}

bool ASquareGameGrid::GetLocationFromTile(FGridTile Tiledata ,FVector & OutLocation, bool Center) const
{

	if (!IsTileValid(Tiledata.row, Tiledata.column))
	{
		OutLocation = FVector(0.0f, 0.0f, 0.0f);
		return(false);
	}
	
	OutLocation.Z = GetGridElevation(Tiledata);

	FVector gridstartlocation = GetActorLocation();

	if (Center)
	{
		float centeroffset = TileSize / 2.0f;
		OutLocation.X = gridstartlocation.X + (Tiledata.row * TileSize) + centeroffset;
		OutLocation.Y = gridstartlocation.Y + (Tiledata.column * TileSize) + centeroffset;
	}
	else
	{
		OutLocation.X = gridstartlocation.X + (Tiledata.row * TileSize);
		OutLocation.Y = gridstartlocation.Y + (Tiledata.column * TileSize);
	}

	return(true);
}

void ASquareGameGrid::DrawLine(FVector LineStart, FVector LineEnd, float LineThickness, TArray<FVector>& Verts, TArray<int>& Tris)
{
	const float halfthickness = LineThickness / 2.0f;

	/*A Line Segment is made of two triangles forming a rectangle, get the hypotenuse, then cross with z to get the grid direction*/
	const FVector trianglehypotenuse= (LineEnd - LineStart).GetSafeNormal();
	const FVector griddirection =   FVector::CrossProduct(trianglehypotenuse, FVector(0.0f, 0.0f, 1.0f));
	const FVector vertexadjustment = griddirection * halfthickness;

	/*Create and Append the new triangle vertex map so we know which verticies make up the two triangles*/
	int * newtriangles = new int[6]();

	const int numverts = Verts.Num();
	newtriangles[0] = numverts + 2;
	newtriangles[1] = numverts + 1;
	newtriangles[2] = numverts;
	newtriangles[3] = numverts + 2;
	newtriangles[4] = numverts + 3;
	newtriangles[5] = numverts + 1;

	Tris.Append(newtriangles, 6);
	delete[] newtriangles;
	

	/*Create and Append the new Verticies*/

	FVector * newverticies = new FVector[4]();
	newverticies[0] = LineStart + vertexadjustment;
	newverticies[1] = LineEnd + vertexadjustment;
	newverticies[2] = LineStart - vertexadjustment;
	newverticies[3] = LineEnd - vertexadjustment;

	Verts.Append(newverticies, 4);
	delete[] newverticies;
}

void ASquareGameGrid::DrawGrid()
{
	if (GridMaterial)
	{
		LineTriangles.Empty();
		LineVerticies.Empty();

		UMaterialInstanceDynamic * linematerial = UMaterialInstanceDynamic::Create(GridMaterial, this);

		linematerial->SetVectorParameterValue(ColorParameterName, LineColor);
		linematerial->SetScalarParameterValue(OpacityParameterName, LineOpacity);


		/*Obtain Horizontal Line Data*/
		for (int i = 0; i <= NumRows; i++)
		{
			float linestartpos = i * TileSize;
			FVector LineStart = FVector(linestartpos, 0.0f, 0.0f);
			FVector LineEnd = FVector(linestartpos, GetGridWidth(), 0.0f);

			DrawLine(LineStart, LineEnd, GridLineThickness, LineVerticies, LineTriangles);
		}

		/*Obtain Verticle Lines Data*/
		for (int i = 0; i <= NumColumns; i++)
		{
			float linestartpos = i * TileSize;
			FVector LineStart = FVector(0.0f, linestartpos, 0.0f);
			FVector LineEnd = FVector(GetGridHieght(), linestartpos, 0.0f);

			DrawLine(LineStart, LineEnd, GridLineThickness, LineVerticies, LineTriangles);
		}


		if (LinesProceduralMesh)
		{
			LinesProceduralMesh->CreateMeshSection(0, LineVerticies, LineTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
			LinesProceduralMesh->SetMaterial(0, linematerial);
		}

		
		FGridTile testtile1;
		testtile1.column = 0;
		testtile1.row = 0;

		FGridTile testtile2;
		testtile2.column = 1;
		testtile2.row = 1;

		TArray<FGridTile> testselections = TArray<FGridTile>();
		testselections.Emplace(testtile1);
		testselections.Emplace(testtile2);

		SetSelectedTiles(testselections);
	}

}