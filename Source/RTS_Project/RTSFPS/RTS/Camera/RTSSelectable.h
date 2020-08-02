// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTSSelectionComponent.h"
#include "RTSSelectable.generated.h"

UCLASS()
class RTS_PROJECT_API ARTSSelectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTSSelectable(const FObjectInitializer& ObjectInitializer);


public:	
	
	virtual void SetSelected();
	virtual void SetDeselected();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Selection)
	URTSSelectionComponent* Selection;
};
