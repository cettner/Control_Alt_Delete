// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"


#include "GridLayer.h"
#include "GameGrid.h"
#include "GridLayerComponent.generated.h"


UCLASS()
class RTS_PROJECT_API UGridLayerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGridLayerComponent();
	virtual TSet<UGridTile*> GetGridSpace() const;
	virtual void ApplyLayers();

protected:
	virtual AGameGrid* GetGameGrid() const;

protected:
	virtual void InitializeComponent() override;
	virtual void OnRegister() override;
	virtual void PostLoad() override;
	virtual void OnComponentCreated() override;
	virtual void Activate(bool bReset = false) override;

#if WITH_EDITOR
public:
	virtual void PostRootTileChanged();

protected:
	virtual void PostEditComponentMove(bool bFinished) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Layers")
	TArray<TSubclassOf<UGridLayer>> LayerClasses = TArray<TSubclassOf<UGridLayer>>();

protected:
	UPROPERTY(Transient)
	TArray<UGridLayer*> ActiveLayers = TArray<UGridLayer*>();
};
