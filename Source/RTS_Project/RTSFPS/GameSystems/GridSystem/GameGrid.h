// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.h"
#include "GridLayer.h"
#include "ProceduralMeshComponent.h"
#include "NavigationData.h"


#include "Layers/FlowFieldSolutionLayer.h"
#include "GameGrid.generated.h"


struct FVectorFieldPath : FNavigationPath
{
	typedef FNavigationPath Super;
public:
	void InitSolution(UFlowFieldSolutionLayer* InSolution)
	{
		if (InSolution != nullptr)
		{
			SolutionIndex = InSolution->GetLayerID();
			PathPoints.Add(PathFindingQueryData.StartLocation);
			PathPoints.Add(PathFindingQueryData.EndLocation);
			bIsReady = InSolution->IsSolutionReady();
		}
	}
	FORCEINLINE int32 GetSolutionID() const { return SolutionIndex; }

protected:
	int32 SolutionIndex = INVALID_LAYER_ID;
};

struct FSolutionRecalculationRequest
{
	UFlowFieldSolutionLayer * Solution;
	ENavPathUpdateType::Type Reason;

	FSolutionRecalculationRequest(UFlowFieldSolutionLayer * InPath, ENavPathUpdateType::Type InReason)
		: Solution(InPath), Reason(InReason)
	{}

	bool operator==(const FSolutionRecalculationRequest& Other) const { return Solution == Other.Solution; }
};


UCLASS()
class RTS_PROJECT_API AGameGrid : public ANavigationData
{
	GENERATED_BODY()

protected:
	AGameGrid();

public:
	int32 GetMaxTiles() const;
	FORCEINLINE int32 GetMaxRows() const { return NumRows; }
	FORCEINLINE int32 GetMaxCols() const {return NumColomns;}
	const TArray<UGridTile*>& GetTiles() const { return GridData; };
	virtual float GetTileRadius() const;
	virtual float GetTileEdgeLength() const;
	float GetGridWidth() const;
	float GetGridLength() const;
	virtual bool IsValidGridLocation(const FVector& InLocation) const;
	virtual void SetTileVisible(int32 TileID, bool bIsVisble) const;
	virtual void SetTileColor(int32 TileID, FLinearColor InTileColor);
	UGridTile* GetTileFromLocation(const FVector& InLocation) const;
	
	template<typename T>
	T* GetLayerOfClass() const
	{
		T* retval = nullptr;
		for (int32 i = 0; i < GridLayers.Num(); i++)
		{
			if (T* layer = Cast<T>(this->GridLayers[i]))
			{
				retval = layer;
				break;
			}
		}

		return retval;
	}

	template<typename T>
	TArray<T*> GetLayersOfClass() const
	{
		TArray<T*> retval = TArray<T*>();

		for (int i = 0; i < GridLayers.Num(); i++)
		{
			if (T* layer = Cast<T>(this->GridLayers[i]))
			{
				retval.Emplace(layer);
			}
		}

		return retval;
	}

	template<typename T>
	T * GetLayerByIndex(int32 InLayerID) const
	{
		if (InLayerID > INVALID_LAYER_ID && InLayerID < GridLayers.Num())
		{
			return Cast<T>(GridLayers[InLayerID]);
		}
		else
		{
			return nullptr;
		}
	}

public:
	UGridLayer* AddGridLayer(TSubclassOf<UGridLayer> InLayerClass, const TArray<UGridTile*>& InActiveTiles, AActor* InApplicator = nullptr, const bool InbDelayActivation = false);
	bool RemoveGridLayer(UGridLayer* InLayerToRemove);
	void FinishLayerActivation(UGridLayer * InLayer);
	void SetActiveLayer(UGridLayer* InLayer);

public:
	FORCEINLINE FPathFindingResult FindPath(const FNavAgentProperties& AgentProperties, const FVectorFieldQuery& Query) const
	{
		check(FindVectorFieldImplementation);
		// this awkward implementation avoids virtual call overhead - it's possible this function will be called a lot
		return (*FindVectorFieldImplementation)(AgentProperties, Query);
	}
	int32 RegisterObserveredSolution(UFlowFieldSolutionLayer * InObservedPath);
	bool UnRegisterObservationSolution(UFlowFieldSolutionLayer* InObservedPath);

protected:
	virtual void DrawGridLines();
	virtual void DrawGridTiles();
	virtual bool BuildGridData();
	virtual void ComputeGridConstants();
	virtual void AddTileNeighbors();
	virtual void InitializeLayers();
	virtual bool DetermineTileLocation(const int32 InRow, const int32 InCol, FVector& OutTileCenter) const;

protected:
	virtual void BuildLineRenderData(const FVector LineStart, const FVector LineEnd, const float LineThickness, TArray<FVector>& Verts, TArray<int>& Tris);

	/*Navigation*/
protected:

	/*This Function is a redirect from ANavigationData::FindPathImplementation function pointer, this is done to supercede virtual overhead,
	matched with the navmesh implementation*/
	static FPathFindingResult FindVectorField(const FNavAgentProperties& AgentProperties, const FVectorFieldQuery& Query);
	void RequestNavigationRegistration();
	UFlowFieldSolutionLayer* GetSolutionFromQuery(const FVectorFieldQuery& Query) const;
	UFlowFieldSolutionLayer* BuildSolutionFromQuery(const FVectorFieldQuery& Query);
	FPathFindingResult RepathSolution(UFlowFieldSolutionLayer* InSolution, ENavPathUpdateType::Type InRepathReason);
	virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	void PurgeUnusedSolutions();
	/*End Navigation*/
protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PostLoad() override;

protected:
	virtual void DrawDebugData();


protected:
	/*Width of Drawn Line*/
	UPROPERTY(EditAnywhere, Category = "Setup")
	float GridLineThickness = 10.0f;

	/*Tile Radius*/
	UPROPERTY(EditAnywhere, Category = "Setup")
	float TileEdgeLength = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float GridWidth = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float GridLength = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<UGridTile> TileClass = UGridTile::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	UMaterial* GridMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FName ColorParameterName = "Color";

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FLinearColor LineColor;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FName OpacityParameterName = "Opacity";

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float LineOpacity = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	FLinearColor DefaultTileFillColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float DefaultTileFillOpacity = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TArray<TSubclassOf<UGridLayer>> StartupLayers = TArray<TSubclassOf<UGridLayer>>();

	/*Runtime*/
protected:
	UPROPERTY(Transient)
	TArray<UGridTile*> GridData = TArray<UGridTile*>();

	UPROPERTY(Transient)
	TArray<UGridLayer*> GridLayers = TArray<UGridLayer*>();
	TArray<int32> OpenLayerIndices = TArray<int32>();

	bool bIsGridDataBuilt = false;

	UProceduralMeshComponent* LinesProceduralMesh = nullptr;

	UProceduralMeshComponent* SelectionProceduralMesh = nullptr;

	int32 NumRows = 0;
	int32 NumColomns = 0;
	float TileRadius = 0.0f;

protected:
	typedef FPathFindingResult(*FFindVFieldPtr)(const FNavAgentProperties& AgentProperties, const FVectorFieldQuery& Query);
	FFindVFieldPtr FindVectorFieldImplementation;

	TArray<UFlowFieldSolutionLayer *> ObservedSolutions;
	TArray<FSolutionRecalculationRequest> SolutionRepathRequests;

#if WITH_EDITOR

protected:
	virtual void RebuildGridData(bool bRedrawMesh = true);

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	bool bRebuildGridData = true;
#endif

};
