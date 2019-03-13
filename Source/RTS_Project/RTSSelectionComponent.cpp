// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectionComponent.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

// Sets default values for this component's properties
URTSSelectionComponent::URTSSelectionComponent()
{
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Decals/Selection_Decal.Selection_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(300.0f, 300.0f, 300.0f);
	CursorToWorld->RelativeScale3D = FVector(1.0f,.25f,.25f);
	CursorToWorld->SetRelativeLocation(FVector(0, 0, -100.0f));
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	CursorToWorld->SetVisibility(false);
}

// Called when the game starts
void URTSSelectionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URTSSelectionComponent::SetSelected()
{
	CursorToWorld->SetVisibility(true);
}

void URTSSelectionComponent::SetDeselected()
{
	CursorToWorld->SetVisibility(false);
}

void URTSSelectionComponent::SetRoot(USceneComponent * RootComponent)
{
	CursorToWorld->AttachTo(RootComponent);
}

void URTSSelectionComponent::SetDetection(UPrimitiveComponent * Collision)
{
	Collision->SetCollisionResponseToChannel(SELECTION_CHANNEL, ECR_Block);
}
