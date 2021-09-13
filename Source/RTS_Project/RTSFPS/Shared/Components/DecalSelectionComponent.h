// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "DecalSelectionComponent.generated.h"

/**
 * 
 */
#define SELECTION_CHANNEL  ECC_GameTraceChannel1


UCLASS(BluePrintable)
class RTS_PROJECT_API UDecalSelectionComponent : public UDecalComponent
{
	GENERATED_BODY()

public:
		UDecalSelectionComponent();
		void SetDetection(UPrimitiveComponent * Collision);
		void SetSelectionColor(FLinearColor Color);


protected:
	/*Name of Parameter Set in Material*/
	UPROPERTY(EditDefaultsOnly, Category = Selection)
	FName ColorParamName;
	
	UPROPERTY(EditDefaultsOnly, Category = Selection)
	TEnumAsByte<ECollisionChannel> SelectionChannel;
};
