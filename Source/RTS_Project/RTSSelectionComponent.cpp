// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectionComponent.h"
#include "Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "GameAssets.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

// Sets default values for this component's properties
URTSSelectionComponent::URTSSelectionComponent()
{
	PrimarySelectionRing = CreateDefaultSubobject<UDecalComponent>("PrimarySelectionRing");
	static ConstructorHelpers::FObjectFinder<UMaterial> PrimaryDecalMaterialAsset(TEXT(SELECTION_DECAL_PATH));
	if (PrimaryDecalMaterialAsset.Succeeded())
	{
		PrimarySelectionRing->SetDecalMaterial(PrimaryDecalMaterialAsset.Object);
	}
	PrimarySelectionRing->DecalSize = FVector(300.0f, 300.0f, 300.0f);
	PrimarySelectionRing->SetRelativeScale3D(FVector(1.0f,.25f,.25f));
	PrimarySelectionRing->SetRelativeLocation(FVector(0, 0, -100.0f));
	PrimarySelectionRing->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	PrimarySelectionRing->SetVisibility(false);
	
	SecondarySelectionRing = CreateDefaultSubobject<UDecalComponent>("SecondarySelectionRing");
	static ConstructorHelpers::FObjectFinder<UMaterial> SecondaryDecalMaterialAsset(TEXT(ENEMY_DECAL_PATH));
	if (SecondaryDecalMaterialAsset.Succeeded())
	{
		SecondarySelectionRing->SetDecalMaterial(SecondaryDecalMaterialAsset.Object);
	}
	SecondarySelectionRing->DecalSize = FVector(300.0f, 300.0f, 300.0f);
	SecondarySelectionRing->SetRelativeScale3D(FVector(1.0f, .25f, .25f));
	SecondarySelectionRing->SetRelativeLocation(FVector(0, 0, -100.0f));
	SecondarySelectionRing->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	SecondarySelectionRing->SetVisibility(false);
}

// Called when the game starts
void URTSSelectionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URTSSelectionComponent::SetSelected()
{
	PrimarySelectionRing->SetVisibility(true);
}

void URTSSelectionComponent::SetDeselected()
{
	PrimarySelectionRing->SetVisibility(false);
}

void URTSSelectionComponent::EnableSecondary()
{
	SecondarySelectionRing->SetVisibility(true);
}

void URTSSelectionComponent::DisableSecondary()
{
	SecondarySelectionRing->SetVisibility(false);
}

void URTSSelectionComponent::SetRoot(USceneComponent * RootComponent)
{
	PrimarySelectionRing->SetupAttachment(RootComponent);
	SecondarySelectionRing->SetupAttachment(RootComponent);
}

void URTSSelectionComponent::SetDetection(UPrimitiveComponent * Collision)
{
	Collision->SetCollisionResponseToChannel(SELECTION_CHANNEL, ECR_Block);
}
