// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectable.h"


// Sets default values
ARTSSelectable::ARTSSelectable(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);

	Selection = ObjectInitializer.CreateDefaultSubobject<URTSSelectionComponent>(this, TEXT("Selection"));
	AddOwnedComponent(Selection);

	Selection->SetRoot(RootComponent);
	Selection->SetDetection(Mesh);

}

// Called when the game starts or when spawned
void ARTSSelectable::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARTSSelectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARTSSelectable::SetSelected()
{
	Selection->SetSelected();
}

void ARTSSelectable::SetDeselected()
{
	Selection->SetDeselected();
}

