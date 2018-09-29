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

// Called every frame
void AResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AResource::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(true);
}

void AResource::OnRightClick(AActor* Target, FKey ButtonPressed)
{

	if (ButtonPressed == EKeys::RightMouseButton)
	{
		ARTSPlayerController * PC =	(ARTSPlayerController*)GetWorld()->GetFirstPlayerController();

		for (int i = 0; i < PC->SelectedUnits.Num(); i++)
		{
			if (Cast<ARTSBUILDER>(PC->SelectedUnits[i]))
			{
				Cast<ARTSBUILDER>(PC->SelectedUnits[i])->Set_Node(this);
			}
		}	
	}
}

int AResource::Mine(UINT amount_to_mine)
{
	if (resource_val > amount_to_mine)
	{
		resource_val -= amount_to_mine;
		return(amount_to_mine);
	}
	else // theres not enough to give so give what's left.
	{
		int retval = resource_val;
		resource_val = 0;

		bool die = Destroy(this);
		
		return(retval);
	}


}


