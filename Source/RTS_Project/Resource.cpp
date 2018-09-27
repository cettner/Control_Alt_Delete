// Fill out your copyright notice in the Description page of Project Settings.

#include "Resource.h"
#include "RTSPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"





AResource::AResource(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	OnClicked.AddUniqueDynamic(this, &AResource::OnRightClick);
}


// Called when the game starts or when spawned
void AResource::BeginPlay()
{
	Super::BeginPlay();
	
}

void AResource::OnRightClick(AActor* Target, FKey ButtonPressed)
{

	if (ButtonPressed == EKeys::RightMouseButton)
	{
		ARTSPlayerController * PC =	(ARTSPlayerController*)GetWorld()->GetFirstPlayerController();
		Tasked_Units = PC->SelectedUnits;

		if (Tasked_Units.Num() > 0)
		{
			int hit = 12;
		}
	}
	
}



// Called every frame
void AResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

