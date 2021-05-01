// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SquareGameGrid.generated.h"


USTRUCT()
struct FGridTile
{
	GENERATED_USTRUCT_BODY()

	int row = -1;
	int column = -1;
	FVector TileCenter = FVector();
	bool IsValid = false;
};

UCLASS()
class RTS_PROJECT_API ASquareGameGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	ASquareGameGrid();

protected:
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform & Transform) override;


public:
	
	void SetSelectedTiles(TArray<FGridTile> SelectedTiles);
	float GetGridWidth() const;
	float GetGridHieght() const;
	virtual float GetGridElevation(FGridTile Tiledata) const;

	bool IsTileValid(int Row, int Column) const;
	FGridTile GetTileFromLocation(FVector Location) const;
	bool GetLocationFromTile(FGridTile Tiledata, FVector & OutLocation, bool Center = true) const;

protected:
	virtual void DrawLine(FVector LineStart, FVector LineEnd, float LineThickness, TArray<FVector>& Verts, TArray<int>& Tris);
	virtual void DrawGrid();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int NumRows = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int NumColumns = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TileSize = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float GridLineThickness = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FLinearColor LineColor;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FLinearColor SelectionColor;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LineOpacity = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float SelectionOpacity = 1.0f;


protected:
	UProceduralMeshComponent * LinesProceduralMesh;

	UProceduralMeshComponent * SelectionProceduralMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Procedural Mesh")
	UMaterial * GridMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Procedural Mesh")
	FName ColorParameterName = "Color";

	UPROPERTY(EditDefaultsOnly, Category = "Procedural Mesh")
	FName OpacityParameterName = "Opacity";

	UPROPERTY(BlueprintReadOnly)
	TArray<int> LineTriangles = TArray<int>();

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> LineVerticies = TArray<FVector>();




};