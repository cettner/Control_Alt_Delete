// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSSelectable.h"


// Sets default values
ARTSSelectable::ARTSSelectable(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Selection = ObjectInitializer.CreateDefaultSubobject<URTSSelectionComponent>(this, TEXT("Selection"));
	Selection->SetRoot(RootComponent);
}



void ARTSSelectable::SetSelected()
{
	Selection->SetSelected();
}

void ARTSSelectable::SetDeselected()
{
	Selection->SetDeselected();
}

