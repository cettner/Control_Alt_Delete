// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectionComponent.h"
#include "RTS_Project/AssetHelpers/GameAssets.h"

#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

// Sets default values for this component's properties
URTSSelectionComponent::URTSSelectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ColorParamName = "Color";
	PrimarySelectionRing = CreateDefaultSubobject<UDecalComponent>("PrimarySelectionRing");

	if (IsValid(DecalMaterial))
	{
		PrimarySelectionRing->SetDecalMaterial(DecalMaterial);
	}

	PrimarySelectionRing->DecalSize = FVector(300.0f, 300.0f, 300.0f);
	PrimarySelectionRing->SetRelativeScale3D(FVector(1.0f, .25f, .25f));
	PrimarySelectionRing->SetRelativeLocation(FVector(0, 0, -100.0f));
	PrimarySelectionRing->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	PrimarySelectionRing->SetHiddenInGame(true);
}

void URTSSelectionComponent::SetSelected()
{
	PrimarySelectionRing->SetHiddenInGame(false);
}

void URTSSelectionComponent::SetDeselected()
{
	PrimarySelectionRing->SetHiddenInGame(true);
}

void URTSSelectionComponent::SetRoot(USceneComponent * RootComponent)
{
	PrimarySelectionRing->SetupAttachment(RootComponent);
}

void URTSSelectionComponent::SetDetection(UPrimitiveComponent * Collision)
{
	Collision->SetCollisionResponseToChannel(SELECTION_CHANNEL, ECR_Block);
}

void URTSSelectionComponent::SetSelectionColor(FLinearColor Color)
{
	if (PrimarySelectionRing->GetDecalMaterial()->IsA(UMaterialInstanceDynamic::StaticClass()))
	{
		UMaterialInstanceDynamic * DynamicColor = 	Cast<UMaterialInstanceDynamic>(PrimarySelectionRing->GetDecalMaterial());
		DynamicColor->SetVectorParameterValue(ColorParamName, Color);
	}
	else
	{
		UMaterialInstanceDynamic * DynamicColor = UMaterialInstanceDynamic::Create(PrimarySelectionRing->GetDecalMaterial(), PrimarySelectionRing);
		if (DynamicColor && PrimarySelectionRing)
		{
			DynamicColor->SetVectorParameterValue(ColorParamName, Color);
			PrimarySelectionRing->SetDecalMaterial(DynamicColor);
		}
	}
}
