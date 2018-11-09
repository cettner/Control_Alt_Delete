// Fill out your copyright notice in the Description page of Project Settings.
#include "RTSStructure.h"
#include "GameFramework/PlayerController.h"
#include "Components/DecalComponent.h"
#include "Materials/Material.h"


// Sets default values
ARTSStructure::ARTSStructure(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Empty = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	//Empty->AttachTo(RootComponent);
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));

	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	OnClicked.AddUniqueDynamic(this, &ARTSStructure::OnClick);

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(300.0f, 300.0f, 300.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	CursorToWorld->SetVisibility(false);
}

// Called when the game starts or when spawned
void ARTSStructure::BeginPlay()
{
	PC = (ARTSPlayerController*)GetWorld()->GetFirstPlayerController();
	HudPtr = Cast<ARTSHUD>(PC->GetHUD());
	bIsConstructed = false;
	CurrentIntegrity = 1.0;
}

void ARTSStructure::OnClick(AActor * Target, FKey ButtonPressed)
{
	if (HudPtr->state == ARTSHUD::RTS_SELECT_AND_MOVE)
	{
		if (ButtonPressed == EKeys::LeftMouseButton)
		{
			if (HudPtr->Selected_Structure.Find(this))
			{
				HudPtr->StructureSelected = true;
				HudPtr->Selected_Structure.Add(this);
			}

		}
		else if (ButtonPressed == EKeys::RightMouseButton)
		{
			for (int i = 0; i < PC->SelectedUnits.Num(); i++)
			{
				if (Cast<ARTSBUILDER>(PC->SelectedUnits[i]))
				{
					Cast<ARTSBUILDER>(PC->SelectedUnits[i])->ReleaseAssets();
					Cast<ARTSBUILDER>(PC->SelectedUnits[i])->Set_Structure(this);
				}
			}
		}
	}
}
// Called every frame
void ARTSStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARTSStructure::SetSelected()
{
	CursorToWorld->SetVisibility(true);
}

void ARTSStructure::SetDeselected()
{
	CursorToWorld->SetVisibility(false);
}

bool ARTSStructure::IsDropPoint()
{
	return (true);
}

