// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "RTSSelectionComponent.generated.h"

#define SELECTION_CHANNEL  ECC_GameTraceChannel1

enum SELECTION_TYPE
{
	CLICK_AND_CLEAR
};

UCLASS(Blueprintable)
class RTS_PROJECT_API URTSSelectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URTSSelectionComponent();

	void SetSelected();
	void SetDeselected();
	void SetRoot(USceneComponent* RootComponent);
	void SetDetection(UPrimitiveComponent * Collision);
	void SetSelectionColor(FLinearColor Color);

protected:
	/*TODO Find a Way to Set Material for Selection Ring from blueprint (Currently Hardcoded Reference)*/
	UPROPERTY(EditDefaultsOnly, Category = Selection)
	UDecalComponent* PrimarySelectionRing;

	/*Name of Parameter Set in Material*/
	UPROPERTY(EditDefaultsOnly, Category = Selection)
	FName ColorParamName;

	/**/
	UPROPERTY(EditDefaultsOnly)
	UMaterial * DecalMaterial;
};
