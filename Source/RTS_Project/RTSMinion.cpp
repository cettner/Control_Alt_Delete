// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RTSMinion.h"
#include "UObject/ConstructorHelpers.h"
#include "RTSSelectionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "RTSAIController.h"
#include"Runtime/Engine/Classes/Engine/World.h"
#include "Commander.h"

ARTSMinion::ARTSMinion()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	static ConstructorHelpers::FObjectFinder<UTexture> ThumbnailAsset(TEXT("Texture2D'/Game/Pictures/UI_Thumbnails/Builder_PNG.Builder_PNG'"));
	if (ThumbnailAsset.Succeeded())
	{
		Thumbnail = ThumbnailAsset.Object;
	}

	//AI recives Access on Spawn
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator();
	team_index = Iterator.GetIndex();

	Selection = CreateDefaultSubobject<URTSSelectionComponent>(TEXT("Selection"));
	Selection->SetRoot(RootComponent);
	Selection->SetDetection(GetCapsuleComponent());
}

AActor * ARTSMinion::GetTarget()
{
	return(TargetActor);
}

void ARTSMinion::SetTarget(AActor * NewTarget)
{
	ARTSAIController * rtscontrol = Cast<ARTSAIController>(GetController());
	if (rtscontrol)
	{
		rtscontrol->SetTarget(NewTarget);
	}
}

void ARTSMinion::SetSelected()
{
	Selection->SetSelected();
}

void ARTSMinion::SetDeselected()
{
	Selection->SetDeselected();
}

ACommander * ARTSMinion::GetCommander()
{
	return(nullptr);
}

void ARTSMinion::ReleaseAssets(FVector Order_Local)  //This function allows the unit to ignore the PC or dispatch any remaining tasks before enacting the move operation.
{
	if (bismovespecial) // we have special instructions from elsewhere so ignore the PC this time.
	{
		bismovespecial = false;
	}
	else  //unless overloaded this should never be called
	{
		RtsMove(Order_Local);
	}
	
}

//interface function for override;
void ARTSMinion::ReleaseAssets()
{
	if (bismovespecial) // we have special instructions from elsewhere so ignore the PC this time.
	{
		bismovespecial = false;
	}
}

bool ARTSMinion::HasAssets()
{
	return (bismovespecial);
}

void ARTSMinion::RtsMove(FVector Local)
{
	ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
	if (AIC != NULL)
	{
		AIC->MoveToLocation(Local, 5.0f, false, true, true, true, 0, false);
	}
}

void ARTSMinion::RtsMoveToActor(AActor * move_to_me)
{
	ARTSAIController * AIC = Cast<ARTSAIController>(GetController());
	if (AIC != NULL)
	{
		AIC->MoveToActor(move_to_me, 5.0f, true, false, false, 0, false);
	}
}

void ARTSMinion::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ARTSMinion::BeginPlay()
{
	Super::BeginPlay();
	TargetLocation = GetActorLocation();
}

UTexture * ARTSMinion::GetThumbnail()
{
	return Thumbnail;
}
