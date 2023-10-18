// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#if WITH_EDITOR
#include "Engine/World.h"
#endif

#include "GameGrid.h"
#include "GridLayerComponent.h"
#include "GridAttachmentActor.generated.h"

UCLASS()
class RTS_PROJECT_API AGridAttachmentActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridAttachmentActor();

	virtual void SetRootTile(UGridTile* InRootTile);
	AGameGrid* GetGameGrid() const;
	FORCEINLINE UGridTile * GetRootTile() const{ return RootTile; }

protected:
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

protected: 
	AGameGrid* FindGrid(UWorld * InWorld);

protected:
	UPROPERTY(EditAnywhere)
	UGridLayerComponent* GridLayerComp = nullptr;

protected:
	UGridTile * RootTile = nullptr;

#if WITH_EDITOR
public:
	virtual void OnGridConstructed(AGameGrid * InGrid);

protected:
	virtual void PostRootTileChanged();

protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif
};
